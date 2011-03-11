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
#include "../headers/Timer.h"
#include "../headers/UDPSocket.h"
#include "../headers/UDPSocketCallback.h"
#include "Utils.h"

using namespace std;

class Callback : public UDPSocketCallback {
public:
    Callback() : UDPSocketCallback(), count_(0) {
        sem_.init(0);
    }

    void receive(AddressPort& ap, unsigned char* buffer, size_t length) {

        timer_.start();

        if(!strcmp((const char*)buffer, "stop")) {
            sem_.post();
            return;
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

    int get_count() {
        return count_;
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
    const char * max_run_time = "max-run-time";

    // setup options
    static struct option long_options[] = {
        {sink_address, required_argument, NULL, 1},
        {sink_port, required_argument, NULL, 2},
        {max_run_time, required_argument, NULL, 3},
        {0, 0, 0, 0}
    };

    OptionParser::instance().parse(argc, argv, long_options);

    string sink_address_ = "127.0.0.1";
    u_int16_t sink_port_ = 5000;
    int max_run_time_ = 30;

    if (OptionParser::instance().present(sink_address)) {
        sink_address_ = OptionParser::instance().argument(sink_address);
    }
    if (OptionParser::instance().present(sink_port)) {
        sink_port_ = atoi(OptionParser::instance().argument(sink_port));
    }
    if (OptionParser::instance().present(max_run_time)) {
        max_run_time_ = atoi(OptionParser::instance().argument(max_run_time));
    }

    cout << "Sink Address:\t" << sink_address_ << endl;
    cout << "Sink Port:\t" << sink_port_ << endl;
    cout << "Max Run Time:\t" << max_run_time_ << endl;

    AddressPort sink(sink_address_, sink_port_);
    UDPSocket socket;
    socket.bind_socket(sink);

    Callback callback;
    socket.receive(&callback);

    cout << "Waiting for signal to stop listening." << endl;
    struct timespec ts;
    Utils::get_timespec_future_time(max_run_time_, 0, &ts);
    bool timedout = callback.get_semaphore().timed_wait(&ts);

    cout << "Done: ";
    if(timedout) {
        cout << "Timed Out" << endl;
    }
    else {
        cout << "Received Signal" << endl;
    }
    cout << "Duration (sec):\t" << callback.get_timer().get_duration_seconds() << endl;
    cout << "Count:\t" << callback.get_count() << endl;


    return (EXIT_SUCCESS);
}

