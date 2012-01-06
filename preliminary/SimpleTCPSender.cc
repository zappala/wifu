// socket, bind, listen, connect, then send data

#include "../applib/wifu_socket.h"
#include "AddressPort.h"
#include "defines.h"
#include <iostream>
#include "OptionParser.h"
#include <stdlib.h>
#include <bits/basic_string.h>
#include "Timer.h"
#include "headers/ISocketAPI.h"
#include "headers/WiFuSocketAPI.h"
#include "headers/KernelSocketAPI.h"
#include <errno.h>
#include <list>
#include <algorithm>
#include "Utils.h"
#include "Semaphore.h"
#include "../test/headers/RandomStringGenerator.h"

using namespace std;

#define optionparser OptionParser::instance()

struct sending_data {
    ISocketAPI* api;
    int chunk;
    Semaphore* mutex;
    AddressPort* peer;
    int connection;
    Semaphore* flag;
    gcstring message;
    pthread_barrier_t* barrier;
};

void my_itoa(int val, char* buffer) {
    sprintf(buffer, "%d", val);
}

bool exists(char* filename) {
    ifstream temp(filename);
    return temp;
}

void* sending_thread(void*);

int main(int argc, char** argv) {

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";

    gcstring portarg = "port";
    int port = 5002;

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = TCP_TAHOE;

    gcstring num = "num";
    int num_bytes = 1000;

    gcstring chunkarg = "chunk";
    // The number of bytes we are willing to send each time we call send().
    int chunk = 10000;


    gcstring threads = "threads";
    int num_threads = 1;

    static struct option long_options[] = {
        {bindaddr.c_str(), required_argument, NULL, 0},
        {portarg.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        {protocolarg.c_str(), required_argument, NULL, 0},
        {chunkarg.c_str(), required_argument, NULL, 0},
        {threads.c_str(), required_argument, NULL, 0},
        {num.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(bindaddr)) {
        hostaddr = optionparser.argument(bindaddr);
    } else {
        cout << "Binding address required!\n";
        cout << "Use option --address <addr>\n";
        return -1;
    }
    if (optionparser.present(portarg)) {
        port = atoi(optionparser.argument(portarg).c_str());
    }

    if (optionparser.present(apiarg)) {
        gcstring api_type = optionparser.argument(apiarg);
        if (!api_type.compare("kernel")) {
            api = new KernelSocketAPI();
        }
    }

    if (optionparser.present(protocolarg)) {
        protocol = atoi(optionparser.argument(protocolarg).c_str());
    }

    if (optionparser.present(num)) {
        num_bytes = atoi(optionparser.argument(num).c_str());
    }

    if (optionparser.present(chunkarg)) {
        chunk = atoi(optionparser.argument(chunkarg).c_str());
    }

    if (optionparser.present(threads)) {
        num_threads = atoi(optionparser.argument(threads).c_str());
    }

    cout << bindaddr << " " << hostaddr << endl;
    cout << portarg << " " << port << endl;
    cout << apiarg << " " << api->get_type() << endl;
    cout << protocolarg << " " << protocol << endl;
    cout << num << " " << num_bytes << endl;
    cout << chunkarg << " " << chunk << endl;
    cout << threads << " " << num_threads << endl;

    // try to open a file containing all the data
    // if it fails, write the file , then try again
    char filename[100];
    my_itoa(num_bytes, filename);
    strncat(filename, ".data", 5);

    if (!exists(filename)) {
        ofstream f(filename);
        f << RandomStringGenerator::get_data(num_bytes);
        f.close();
    }

    gcstring message;
    message.reserve(num_bytes);

    ifstream myfile(filename);
    if (myfile.is_open()) {
        while (myfile.good()) {
            getline(myfile, message);
        }
    }

    int server = api->custom_socket(AF_INET, SOCK_STREAM, protocol);
    if (server <= 0) {
        perror("socket");
    }

    AddressPort to_bind(hostaddr, port);
    int result = api->custom_bind(server, (const struct sockaddr*) to_bind.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    if (result < 0) {
        perror("bind");
    }

    result = api->custom_listen(server, num_threads);
    if (result < 0) {
        perror("listen");
    }

    pthread_t pthreads[num_threads];
    pthread_barrier_t barrier;
    if (pthread_barrier_init(&barrier, NULL, num_threads)) {
        perror("Error creating barrier");
        exit(EXIT_FAILURE);
    }
    
    struct sending_data data[num_threads];


    Semaphore* mutex = new Semaphore();
    mutex->init(1);

    for (int i = 0; i < num_threads; ++i) {
        data[i].barrier = &barrier;
        data[i].api = api;
        data[i].message = message;
        data[i].chunk = chunk;
        data[i].mutex = mutex;
        data[i].flag = new Semaphore();
        data[i].flag->init(0);
        if (pthread_create(&(pthreads[i]), NULL, &sending_thread, &(data[i])) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }
    }

    int i = 0;

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    while (connection = api->custom_accept(server, (struct sockaddr*) &addr, &length)) {
        data[i].peer = new AddressPort(&addr);
        data[i].connection = connection;
        data[i].flag->post();

        if (++i >= num_threads) {
            // done accepting new clients
            break;
        }
    }


    // join threads
    for (int j = 0; j < num_threads; ++j) {
        pthread_join(pthreads[j], NULL);
    }

    api->custom_close(server);

    sleep(4);
}

void* sending_thread(void* arg) {

    struct sending_data* data = (struct sending_data*) arg;
    ISocketAPI* api = data->api;
    int chunk = data->chunk;
    Semaphore* mutex = data->mutex;
    gcstring message = data->message;
    pthread_barrier_t* barrier = data->barrier;

    Timer send_timer;
    int index = 0;
    int sent = 0;
    const char* ptr;
    int length = message.length();

    // Wait for notification to start sending
    data->flag->wait();

    int value = pthread_barrier_wait(barrier);
    if (value != 0 && value != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("Could not wait on barrier before connecting");
        exit(EXIT_FAILURE);
    }

    AddressPort* peer = data->peer;
    int connection = data->connection;

    send_timer.start();

    while (index < length) {

        if (index + chunk > length) {
            chunk = length - index;
        }
        ptr = message.data() + index;

        sent = api->custom_send(connection, ptr, chunk, 0);
        index += sent;
    }
    send_timer.stop();
    api->custom_close(connection);

    mutex->wait();
    cout << "Duration (us) to send " << index << " bytes to " << peer->to_s() << ": " << send_timer.get_duration_microseconds() << endl;
    mutex->post();
}
