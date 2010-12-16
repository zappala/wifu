/* 
 * File:   udp_blaster.cc
 * Author: rbuck
 *
 * Created on December 16, 2010, 11:03 AM
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

/*
 * 
 */
int main(int argc, char** argv) {

    const char * duration = "duration";
    const char * sink_address = "sink-address";
    const char * sink_port = "sink-port";
    const char * sleep_duration = "sleep";
    const char * message_length = "message_length";


    // setup options
    static struct option long_options[] = {
        {duration, required_argument, NULL, 0},
        {sink_address, required_argument, NULL, 1},
        {sink_port, required_argument, NULL, 2},
        {sleep_duration, required_argument, NULL, 3},
        {message_length, required_argument, NULL, 4},
        {0, 0, 0, 0}
    };

    OptionParser options;
    options.parse(argc, argv, long_options);

    int duration_ = 10;
    string sink_address_ = "127.0.0.1";
    u_int16_t sink_port_ = 5000;
    int sleep_ = 10;
    int message_length_ = 1452;

    if (options.present(duration)) {
        duration_ = atoi(options.argument(duration));
    }
    if (options.present(sink_address)) {
        sink_address_ = options.argument(sink_address);
    }
    if (options.present(sink_port)) {
        sink_port_ = atoi(options.argument(sink_port));
    }
    if (options.present(sleep_duration)) {
        sleep_ = atoi(options.argument(sleep_duration));
    }
    if (options.present(message_length)) {
        message_length_ = atoi(options.argument(message_length));
    }

    cout << "Sink Address:\t" << sink_address_ << endl;
    cout << "Sink Port:\t" << sink_port_ << endl;
    cout << "Duration:\t" << duration_ << endl;
    cout << "Sleep:\t\t" << sleep_ << endl;
    cout << "Message Length:\t" << message_length_ << endl;

    string message = "";

    for(int i = 0; i < message_length_; i++) {
        message.append("m");
    }

    AddressPort sink(sink_address_, sink_port_);
    UDPSocket socket;
    socket.makeNonBlocking();

    Timer timer;
    timer.start();
    
    int count = 0;
    while (timer.get_duration_seconds() < duration_) {
        socket.send(sink, message);
        ++count;
        timer.update_stop();
    }

    cout << "Sent " << count << " packets in " << timer.get_duration_seconds() << " seconds" << endl;
        
    sleep(sleep_);

    cout << "Sending termination signal to sink." << endl;
    message = "stop";
    socket.send(sink, message);
    
    return (EXIT_SUCCESS);
}

