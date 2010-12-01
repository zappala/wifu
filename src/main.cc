/* 
 * File:   main.cc
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:47 PM
 */

#include <stdlib.h>
#include <iostream>

#include "Queue.h"
#include "PriorityQueue.h"
#include "TimeoutEvent.h"
#include "TimeoutEventManager.h"
#include "Semaphore.h"
#include "CanceledEvents.h"
#include "Dispatcher.h"
#include "ConnectionManager.h"
#include "ConnectEvent.h"
#include "Socket.h"
#include "UDPInterface.h"
#include "ConnectionManager.h"
#include "defines.h"
#include "PacketReceivedEvent.h"

using namespace std;

/*
 * 
 */


int main(int argc, char** argv) {

    string address("localhost");
    int port = 5000;
    int sleep_time = 10;

    AddressPort ap(address, port);

    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Load Modules
    UDPInterface::instance().start(ap);

    Dispatcher::instance().map_event(type_name(SendPacketEvent), &UDPInterface::instance());
    Dispatcher::instance().map_event(type_name(TimeoutEvent), &TimeoutEventManager::instance());
    Dispatcher::instance().map_event(type_name(CancelTimerEvent), &TimeoutEventManager::instance());

    sleep(sleep_time);

    cout << "hi" << endl;

    return (EXIT_SUCCESS);
}

