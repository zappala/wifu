/* 
 * File:   main.cc
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:47 PM
 */

#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdexcept>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include "Queue.h"
#include "PriorityQueue.h"
#include "events/TimeoutEvent.h"
#include "TimeoutEventManager.h"
#include "Semaphore.h"
#include "events/CanceledEvents.h"
#include "Dispatcher.h"
#include "events/ConnectEvent.h"
#include "Socket.h"
#include "UDPInterface.h"
#include "defines.h"
#include "WifuEndBackEndLibrary.h"
#include "MainSemaphore.h"
#include "ProtocolManager.h"
#include "SimpleTCP.h"

#include "events/SendPacketEvent.h"
#include "events/SendPacketEvent.h"
#include "events/NetworkSendPacketEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/TimeoutEvent.h"
#include "events/TimerFiredEvent.h"
#include "events/AcceptEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "NetworkInterface.h"
#include "TCPPacketFactory.h"
#include "OptionParser.h"
#include "NetworkInterfaceFactory.h"
#include "StandardNetworkInterfaceCreator.h"
#include "MockNetworkInterfaceCreator.h"

#include "pantheios/pantheios.hpp"      /* The root header for Panthieos when using the C++-API. */
#include "pantheios/inserters.hpp"      /* Includes all headers for inserters, incl. integer, real, character */
#include "pantheios/frontends/stock.h"  /* Declares the process identity symbol PANTHEIOS_FE_PROCESS_IDENTITY */
#include "pantheios/backends/bec.file.h"      // be.file header

const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = "wifu-end";
#define PantheiosString(x) PANTHEIOS_LITERAL_STRING(x)

using namespace std;

#define dispatcher Dispatcher::instance()

void main_signal_manager(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            MainSemaphore::instance().post();
    }
}

void register_signals() {
    signal(SIGINT, main_signal_manager);
    signal(SIGQUIT, main_signal_manager);
    signal(SIGTERM, main_signal_manager);
}

void register_protocols() {
    // TODO: figure out a better way to register protocols via a config file
    ProtocolManager::instance().support(SIMPLE_TCP);
}

void setup_network_interface(string& type) {
    if (type == "standard") {
        cout << "Using standard network interface" << endl;
        NetworkInterfaceFactory::instance().set_creator(new StandardNetworkInterfaceCreator());
    }
    else if (type == "mock") {
        cout << "Using mock network interface" << endl;
        NetworkInterfaceFactory::instance().set_creator(new MockNetworkInterfaceCreator());
    }
}

int main(int argc, char** argv) {
    GC_INIT();

    // The following line does not have to be THE first call to the logger; it can be anywhere and the logger will still recognize it and use the filename
    pantheios_be_file_setFilePath(PantheiosString("wifu-end.log"), PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_ALL);
    pantheios::log_DEBUG("main(", pantheios::args(argc, argv), ")");
    pantheios::log_INFORMATIONAL("Welcome");

    //TODO: Change second argument to 0 once we have a logger in place
    daemon(1, 1);

    string network_type = "standard";
    string network = "network";

    static struct option long_options[] = {
        {network.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };
    OptionParser op;
    op.parse(argc, argv, long_options);
    if (op.present(network.c_str())) {
        network_type = op.argument(network.c_str());
    }


    setup_network_interface(network_type);

    MainSemaphore::instance().init(0);

    register_signals();
    register_protocols();

    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Start Back end
    WifuEndBackEndLibrary::instance();

    // Load Modules
    INetworkInterface* network_interface = &(NetworkInterfaceFactory::instance().create());
    network_interface->register_protocol(SIMPLE_TCP, new TCPPacketFactory());
    network_interface->start();

    dispatcher.map_event(type_name(NetworkSendPacketEvent), network_interface);
    dispatcher.map_event(type_name(TimeoutEvent), &TimeoutEventManager::instance());
    dispatcher.map_event(type_name(CancelTimerEvent), &TimeoutEventManager::instance());

    dispatcher.map_event(type_name(SocketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(BindEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ListenEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ConnectEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(AcceptEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &SimpleTCP::instance());


    dispatcher.map_event(type_name(ResponseEvent), &WifuEndBackEndLibrary::instance());

    // Wait indefinitely
    MainSemaphore::instance().wait();

    // This probably is not be needed but it's here in case it turns out to be
    // Closes the file by setting the path to NULL
    // pantheios_be_file_setFilePath(NULL, PANTHEIOS_BEID_ALL);

    return (EXIT_SUCCESS);
}

