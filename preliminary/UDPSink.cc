/* 
 * File:   UDPSink.cc
 * Author: rbuck
 *
 * Created on December 16, 2010, 12:04 PM
 */

#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>

#include "../headers/OptionParser.h"
#include "../headers/AddressPort.h"
#include "Timer.h"
#include "../headers/UDPSocket.h"

using namespace std;

class Callback : public UDPSocketCallback {
public:
    Callback() : UDPSocketCallback(), count_(0) {
        sem_.init(0);
        timer_.start();
    }

    void receive(AddressPort& ap, unsigned char* buffer, size_t length) {

        timer_.update_start();

        if(!strcmp((const char*)buffer, "stop")) {
            sem_.post();
        }



        ++count_;
        timer_.update_stop();
    }

    Semaphore& get_semaphore() {
        return sem_;
    }

    Timer& get_timer() {
        return timer_;
    }

private:
    Timer timer_;
    int count_;
    Semaphore sem_;
};


/*
 * 
 */
int main(int argc, char** argv) {

    const char * sink_address = "sink-address";
    const char * sink_port = "sink-port";

    // setup options
    static struct option long_options[] = {
        {sink_address, required_argument, NULL, 1},
        {sink_port, required_argument, NULL, 2},
        {0, 0, 0, 0}
    };

    OptionParser options;
    options.parse(argc, argv, long_options);

    string sink_address_ = "127.0.0.1";
    u_int16_t sink_port_ = 5000;

    if (options.present(sink_address)) {
        sink_address_ = options.argument(sink_address);
    }
    if (options.present(sink_port)) {
        sink_port_ = atoi(options.argument(sink_port));
    }

    cout << "Sink Address:\t" << sink_address_ << endl;
    cout << "Sink Port:\t" << sink_port_ << endl;

    AddressPort sink(sink_address_, sink_port_);
    UDPSocket socket;
    socket.makeNonBlocking();
    socket.bind_socket(sink);


    Callback callback;
    socket.receive(&callback);
    callback.get_semaphore().wait();

    cout << "Done" << endl;


    return (EXIT_SUCCESS);
}

