//#include "../applib/wifu_socket.h"

#include <iostream>
#include <stdlib.h>
#include <bits/basic_string.h>
#include "../applib/wifu_socket.h"
#include "defines.h"
#include "AddressPort.h"
#include "OptionParser.h"
#include "../test/headers/RandomStringGenerator.h"
#include "Timer.h"
#include "headers/ISocketAPI.h"
#include "headers/WiFuSocketAPI.h"
#include "headers/KernelSocketAPI.h"
#include "Utils.h"
#include "Semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>

using namespace std;

#define optionparser OptionParser::instance()


struct sending_data {
    gcstring dest;
    int port;
    ISocketAPI* api;
    int protocol;
    int chunk;
    Semaphore flag;
    Semaphore go;
    gcstring message;
    Semaphore* mutex;
};


void* sending_thread(void*);

void my_itoa(int val, char* buffer) {
    sprintf(buffer, "%d", val);
}


bool exists(char* filename) {
    bool val = false;
    ifstream temp(filename);
    if(temp) {
        val = true;
    }
    temp.close();
}


int main(int argc, char** argv) {

    gcstring destination = "destination";
    gcstring destport = "port";
    gcstring dest = "127.0.0.1";
    gcstring num = "num";
    gcstring threads = "threads";
    int port = 5002;
    int num_threads = 1;

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = TCP_TAHOE;

    gcstring chunkarg = "chunk";
    // The number of bytes we are willing to receive each time we call recv().
    int chunk = 10000;

    static struct option long_options[] = {
        {destination.c_str(), required_argument, NULL, 0},
        {destport.c_str(), required_argument, NULL, 0},
        {num.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        {protocolarg.c_str(), required_argument, NULL, 0},
        {chunkarg.c_str(), required_argument, NULL, 0},
        {threads.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(destination)) {
        dest = optionparser.argument(destination);
    } else {
        cout << "Destination argument required!\n";
        cout << "Use option --destination <addr>\n";
        return -1;
    }
    if (optionparser.present(destport)) {
        port = atoi(optionparser.argument(destport).c_str());
    }
    int num_bytes = 1000;
    if (optionparser.present(num)) {
        num_bytes = atoi(optionparser.argument(num).c_str());
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

    cout << destination << " " << dest << endl;
    cout << destport << " " << port << endl;
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
    
    if(!exists(filename)) {
        ofstream f(filename);
        f << RandomStringGenerator::get_data(num_bytes);
        f.close();
    }    
    
    gcstring message;
    message.reserve(num_bytes);
    char buffer[num_bytes + 1];
    FILE* input = fopen(filename, "r");
    fread(buffer, num_bytes, 1, input);
    message.append(buffer);
    
    cout << message.length() << endl;
    return;

    pthread_t pthreads[num_threads];
    struct sending_data data[num_threads];

    Semaphore* mutex = new Semaphore();
    mutex->init(1);

    // create all threads
    for(int i = 0; i < num_threads; ++i) {
        data[i].flag.init(0);
        data[i].go.init(0);
        data[i].api = api;
        data[i].chunk = chunk;
        data[i].dest = dest;
        data[i].port = port + i; // this is how we differentiate flows
        data[i].protocol = protocol;
        data[i].message = message;
        data[i].mutex = mutex;

        if (pthread_create(&(pthreads[i]), NULL, &sending_thread, &(data[i])) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }
    }

    // wait for notification that arguments have been copied and a socket has been created
    for(int i = 0; i < num_threads; ++i) {
        data[i].flag.wait();
    }

    // tell all to go
    for(int i = 0; i < num_threads; ++i) {
        data[i].go.post();
    }

    // wait for threads to finish
    for(int i = 0; i < num_threads; ++i) {
        data[i].flag.wait();
    }

    // tell all to write
    for(int i = 0; i < num_threads; ++i) {
        data[i].go.post();
    }

    for(int i = 0; i < num_threads; ++i) {
        pthread_join(pthreads[i], NULL);
    }

    sleep(4);
}

void* sending_thread(void* arg) {

    struct sending_data* data = (struct sending_data*) arg;
    ISocketAPI* api = data->api;
    int chunk = data->chunk;
    gcstring dest = data->dest;
    gcstring message = data->message;
    int port = data->port;
    int protocol = data->protocol;
    Semaphore* mutex = data->mutex;


    AddressPort to_connect(dest, port);
    int client = api->custom_socket(AF_INET, SOCK_STREAM, protocol);

    int index = 0;

//    list<u_int64_t, gc_allocator<u_int64_t> > starts, ends;
//    list<int, gc_allocator<int> > sizes;

    int sent;

    Timer send_timer;


    data->flag.post();
    data->go.wait();

    int result = api->custom_connect(client, (const struct sockaddr*) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    assert(!result);
    
    send_timer.start();

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

//        starts.push_back(Utils::get_current_time_microseconds_64());
        sent = api->custom_send(client, data, chunk, 0);
//        ends.push_back(Utils::get_current_time_microseconds_64());
//        sizes.push_back(sent);
        index += sent;
    }
    send_timer.stop();
    api->custom_close(client);

    data->flag.post();
    data->go.wait();

    mutex->wait();
//    while (!starts.empty()) {
//        //cout << "send " << starts.front() << " " << ends.front() << " " << sizes.front() << " " << to_connect.to_s() << endl;
//        cout << "send " << starts.front() << " " << ends.front() << " " << sizes.front() << endl;
//        starts.pop_front();
//        ends.pop_front();
//        sizes.pop_front();
//    }
//
    cout << "Duration (us) to send " << index << " bytes to " << to_connect.to_s() << ": " << send_timer.get_duration_microseconds() << endl;
    mutex->post();
}
