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
#include "events/PacketReceivedEvent.h"
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

using namespace std;

#define dispatcher Dispatcher::instance()

void main_signal_manager(int signal) {
    switch(signal) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            MainSemaphore::instance().post();
    }
}

void register_signals(){
    signal(SIGINT, main_signal_manager);
    signal(SIGQUIT, main_signal_manager);
    signal(SIGTERM, main_signal_manager);
}

void register_protocols() {
    // TODO: figure out a better way to register protocols via a config file
    ProtocolManager::instance().support(SIMPLE_TCP);
}

int main(int argc, char** argv) {
	GC_INIT();

    //TODO: Change second argument to 0 once we have a logger in place
    daemon(1,1);

    MainSemaphore::instance().init(0);

    register_signals();
    register_protocols();

    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Start Back end
    WifuEndBackEndLibrary::instance();

    // Load Modules
    NetworkInterface::instance().register_protocol(SIMPLE_TCP, new TCPPacketFactory());
    NetworkInterface::instance().start();

    dispatcher.map_event(type_name(NetworkSendPacketEvent), &NetworkInterface::instance());
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

    return (EXIT_SUCCESS);
}

