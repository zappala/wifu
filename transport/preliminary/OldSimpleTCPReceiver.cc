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

using namespace std;

#define optionparser OptionParser::instance()

struct receiving_data {
    ISocketAPI* api;
    int chunk;
    Semaphore* mutex;
    AddressPort* peer;
    int connection;
    Semaphore* flag;
};

void* receiving_thread(void*);

int main(int argc, char** argv) {

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";

    gcstring portarg = "port";
    int port = 5002;

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = TCP_TAHOE;

    gcstring chunkarg = "chunk";
    // The number of bytes we are willing to receive each time we call recv().
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
    cout << chunkarg << " " << chunk << endl;
    cout << threads << " " << num_threads << endl;


    pthread_t pthreads[num_threads];


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


    struct receiving_data data[num_threads];

    Semaphore* mutex = new Semaphore();
    mutex->init(1);

    for (int i = 0; i < num_threads; ++i) {
        data[i].api = api;
        data[i].chunk = chunk;
        data[i].mutex = mutex;
        data[i].flag = new Semaphore();
        data[i].flag->init(0);
        if (pthread_create(&(pthreads[i]), NULL, &receiving_thread, &(data[i])) != 0) {
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


    cout << "Done creating connections" << endl;

    // join threads
    for (int j = 0; j < num_threads; ++j) {
        pthread_join(pthreads[j], NULL);
    }

    cout << "Done joining threads" << endl;

    api->custom_close(server);

    cout << "Done closing server" << endl;

    sleep(4);
    cout << "Done with receive" << endl;
}

void* receiving_thread(void* arg) {

    struct receiving_data* data = (struct receiving_data*) arg;
    ISocketAPI* api = data->api;
    int chunk = data->chunk;
    Semaphore* mutex = data->mutex;

    int size = chunk + 1;
    char buffer[size];
    gcstring all_received = "";
    int num_received = 0;

    //    list<u_int64_t, gc_allocator<u_int64_t> > starts, ends;
    //    list<int, gc_allocator<int> > sizes;
    int return_value;

    Timer recv_timer;
    num_received = 0;
    //    starts.clear();
    //    ends.clear();

    cout << "Right before wait() in receiving thread" << endl;
    data->flag->wait();

    AddressPort* peer = data->peer;
    int connection = data->connection;    
    cout << "Right after wait() in receiving thread" << endl;

    while (true) {
        //memset(buffer, 0, size);
        //        starts.push_back(Utils::get_current_time_microseconds_64());
        cout << "before recv(): " << return_value << endl;
        return_value = api->custom_recv(connection, buffer, chunk, 0);
        cout << "recv() return value: " << return_value << endl;
        //        ends.push_back(Utils::get_current_time_microseconds_64());
        recv_timer.start();

        //        sizes.push_back(return_value);


        if (return_value == 0) {
            break;
        }
        //all_received.append(buffer);
        num_received += return_value;
    }
    recv_timer.stop();

    api->custom_close(connection);

    mutex->wait();

    //    while (!starts.empty()) {
    //        //cout << "recv " << starts.front() << " " << ends.front() << " " << sizes.front() << " " << remote.to_s() /*Sender*/ << " " << to_bind.to_s() /*Receiver*/ << endl;
    //        cout << "recv " << starts.front() << " " << ends.front() << " " << sizes.front() << endl;
    //        starts.pop_front();
    //        ends.pop_front();
    //        sizes.pop_front();
    //    }

    cout << "Duration (us) to recv " << num_received << " bytes from " << peer->to_s() << ": " << recv_timer.get_duration_microseconds() << endl;
    mutex->post();
}
