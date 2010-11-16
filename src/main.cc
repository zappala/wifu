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
    string port("5000");
//    string socket_file("/tmp/socket_file");
//    SocketSocket s(socket_file);
//
//    s.connect(address);
//
//    Queue<int> q;
//    q.enqueue(9);
//    cout << q.size() << endl;
//    cout << q.dequeue() << endl;

    int sleep_time = 10;

    int socket = 0;



//    Queue<int> ints;
//    Queue<double> doubles;
//
//    cout << "Ints: " << typeid (ints).name() << endl;
//    cout << "Doubles: " << typeid (doubles).name() << endl;


    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Load Modules
    ConnnectionManager cmanager;
    UDPInterface interface(address, atoi(port.c_str()));
    TimeoutEventManager tomanager;

    Dispatcher::instance().map_event(type_name(SendPacketEvent), &cmanager);
    Dispatcher::instance().map_event(type_name(ConnectEvent), &cmanager);
    Dispatcher::instance().map_event(type_name(PacketReceivedEvent), &cmanager);
    
    Dispatcher::instance().map_event(type_name(SendPacketEvent), &interface);

    Dispatcher::instance().map_event(type_name(TimeoutEvent), &tomanager);
    Dispatcher::instance().map_event(type_name(CancelTimerEvent), &tomanager);
    Dispatcher::instance().map_event(type_name(TimerFiredEvent), &cmanager);

    //cmanager.test();

    
//     Try Events through Socket
    Socket s;
    s.connect(address, port);



//    string file("/tmp/file");
//    SocketReliability r(file);
//
//    string message("123456789");
//    r.send_to(file, message);
//    r.send_to(file, file);
//
//    string manager_file("/tmp/manager_file");
//    SocketConnectionManager cmanager(manager_file);
//
//    string manager_message("FromManager");
//    cmanager.send_to(file, manager_message);
//
//    cout << SocketDispatcher::instance().getFile() << endl;

    sleep(sleep_time);

    cout << "hi" << endl;

    return (EXIT_SUCCESS);
}

