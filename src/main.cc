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

using namespace std;

/*
 * 
 */

int main(int argc, char** argv) {



    string address("localhost");
//    string socket_file("/tmp/socket_file");
//    SocketSocket s(socket_file);
//
//    s.connect(address);
//
//    Queue<int> q;
//    q.enqueue(9);
//    cout << q.size() << endl;
//    cout << q.dequeue() << endl;

    int sleep_time = 1;

    int socket = 0;

//    TimeoutEvent * one = new TimeoutEvent(socket, 1, 0);
//    TimeoutEvent * oneplus = new TimeoutEvent(socket, 1, 0);
//    TimeoutEvent * six = new TimeoutEvent(socket, 6, 0);
//    TimeoutEvent * two = new TimeoutEvent(socket, 2, 0);
//    TimeoutEvent * three = new TimeoutEvent(socket, 3, 0);
//    TimeoutEvent * four = new TimeoutEvent(socket, 4, 0);
//    TimeoutEvent * five = new TimeoutEvent(socket, 5, 0);
//
//    TimeoutEventManager manager;
//    manager.start_processing();
//
//
//    Queue<int> ints;
//    Queue<double> doubles;
//
//    cout << "Ints: " << typeid (ints).name() << endl;
//    cout << "Doubles: " << typeid (doubles).name() << endl;



    // Start Dispatcher
    Dispatcher::instance().start_processing();

    // Load Modules
    ConnnectionManager manager;
    Dispatcher::instance().map_event(typeid (ConnectEvent).name(), &manager);


    // Try Events through Socket
    Socket s;
    s.connect(address);


    //QDispatcher qd;
    //

    ////    sleep(1);
    //
//    manager.enqueue(six, true);
//    manager.enqueue(five, true);
//    manager.enqueue(four, true);
//    manager.enqueue(three, true);
//    manager.enqueue(two, true);
//    manager.enqueue(one, true);
//    manager.enqueue(oneplus, true);
//
//
//    manager.cancel(two);
//    manager.cancel(three);
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

