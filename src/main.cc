/* 
 * File:   main.cc
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:47 PM
 */

#include <stdlib.h>
#include <iostream>

#include "LocalSocketFullDuplex.h"
#include "SocketReliability.h"
#include "SocketConnectionManager.h"
#include "SocketDispatcher.h"
#include "SocketSocket.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "TimeoutEvent.h"
#include "TimeoutEventManager.h"
#include "SingletonTemplate.h"
#include "Semaphore.h"
#include "CanceledEvents.h"
#include "Dispatcher.h"
#include "ConnectionManager.h"
#include "ConnectEvent.h"
#include "Socket.h"
#include "UDPInterface.h"

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

    TimeoutEvent * one = new TimeoutEvent(socket, 1, 0);
    TimeoutEvent * oneplus = new TimeoutEvent(socket, 1, 0);
    TimeoutEvent * six = new TimeoutEvent(socket, 6, 0);
    TimeoutEvent * two = new TimeoutEvent(socket, 2, 0);
    TimeoutEvent * three = new TimeoutEvent(socket, 3, 0);
    TimeoutEvent * four = new TimeoutEvent(socket, 4, 0);
    TimeoutEvent * five = new TimeoutEvent(socket, 5, 0);

    TimeoutEventManager manager;
    manager.start_processing();


//    Queue<int> ints;
//    Queue<double> doubles;
//
//    cout << "Ints: " << typeid (ints).name() << endl;
//    cout << "Doubles: " << typeid (doubles).name() << endl;



    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Load Modules
    ConnnectionManager cmanager;
    UDPInterface interface;
    TimeoutEventManager tomanager;

    Dispatcher::instance().map_event(typeid (ConnectEvent).name(), &cmanager);
    Dispatcher::instance().map_event(typeid (SendSynEvent).name(), &interface);
    Dispatcher::instance().map_event(typeid (TimeoutEvent).name(), &tomanager);

    
    // Try Events through Socket
//    Socket s;
//    s.connect(address, port);



    Dispatcher::instance().enqueue(oneplus);
    Dispatcher::instance().enqueue(four);
    Dispatcher::instance().enqueue(two);
    Dispatcher::instance().enqueue(six);
    Dispatcher::instance().enqueue(five);
    Dispatcher::instance().enqueue(three);
    Dispatcher::instance().enqueue(one);
    
////
////
    manager.cancel(two);
    manager.cancel(three);
    //    manager.cancel(four);
    //manager.cancel(one);
    //manager.cancel(oneplus);



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

