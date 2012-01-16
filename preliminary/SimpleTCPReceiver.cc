// will call socket, connect, then receive data

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

struct receiving_data {
    gcstring dest;
    int port;
    ISocketAPI* api;
    int protocol;
    int chunk;
    pthread_barrier_t* barrier;
    Semaphore* mutex;
};


void* receiving_thread(void*);



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

    pthread_t pthreads[num_threads];
    pthread_barrier_t barrier;

    if (pthread_barrier_init(&barrier, NULL, num_threads)) {
        perror("Error creating barrier");
        exit(EXIT_FAILURE);
    }

    struct receiving_data data[num_threads];

    Semaphore* mutex = new Semaphore();
    mutex->init(1);

    // create all threads
    for (int i = 0; i < num_threads; ++i) {
        data[i].barrier = &barrier;
        data[i].api = api;
        data[i].chunk = chunk;
        data[i].dest = dest;
        data[i].port = port;
        data[i].protocol = protocol;
        data[i].mutex = mutex;
    }

    for (int i = 0; i < num_threads; ++i) {
        if (pthread_create(&(pthreads[i]), NULL, &receiving_thread, &(data[i])) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }
    }


    for (int i = 0; i < num_threads; ++i) {
        pthread_join(pthreads[i], NULL);
    }


    sleep(4);

}

void* receiving_thread(void* arg) {

    struct receiving_data* data = (struct receiving_data*) arg;
    ISocketAPI* api = data->api;
    int chunk = data->chunk;
    gcstring dest = data->dest;
    int port = data->port;
    int protocol = data->protocol;
    Semaphore* mutex = data->mutex;
    pthread_barrier_t* barrier = data->barrier;

    Timer receive_timer;
    int return_value = 0;
    int size = chunk + 1;
    char buffer[size];
    int num_received = 0;

    AddressPort to_connect(dest, port);
    int client = api->custom_socket(AF_INET, SOCK_STREAM, protocol);

	if(client < 0) {
		cout << "Error creating socket: " << errno << endl;
		exit(-1);
	}

    int value = pthread_barrier_wait(barrier);
    if (value != 0 && value != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("Could not wait on barrier before connecting");
        exit(EXIT_FAILURE);
    }

    int result = api->custom_connect(client, (const struct sockaddr*) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
	if(result < 0) {
		cout << "Error connecting: " << errno << endl;
		exit(-1);
	}

    assert(!result);


    while (true) {

        // This starting and stoping needs to be this way (stop then start):
        // We don't want to count the last recv() time (that returns 0)
        // basically, we want to quit when we received all the data
        // stop() will be updated each time it is called.
        // We don't want to start counting until we have received data
        // so we miss the first recv call worth of data in the timer.
        // start() is only set the first time it is called
        receive_timer.stop();
        return_value = api->custom_recv(client, buffer, chunk, 0);
        receive_timer.start();
        


	if (return_value > 0) {
		num_received += return_value;
	}
	else if (return_value == 0) {	
		break;
	}
	else {
		cout << "error in receiving: " << errno << endl;
	}        
    }
    
    api->custom_close(client);


    mutex->wait();
    cout << "Duration (us) to recv " << num_received << " bytes from " << to_connect.to_s() << ": " << receive_timer.get_duration_microseconds() << endl;
    mutex->post();
}
