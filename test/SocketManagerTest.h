/* 
 * File:   SocketManagerTest.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 3:59 PM
 */

#ifndef _SOCKETMANAGERTEST_H
#define	_SOCKETMANAGERTEST_H

#include <iostream>
#include <string>
#include <vector>

#include "headers/UnitTest++.h"
#include "../headers/SocketManager.h"

#define manager SocketManager::instance()

using namespace std;

namespace {

    SUITE(SocketManager) {

        TEST(SocketManagerTest) {
            AddressPort* ap0 = new AddressPort("127.0.0.0", 0);
            AddressPort* ap1 = new AddressPort("127.0.0.1", 1);
            AddressPort* ap2 = new AddressPort("127.0.0.2", 2);
            AddressPort* ap3 = new AddressPort("127.0.0.3", 3);

            int protocol = 0;

            // One
            Socket* s0 = new Socket(protocol, ap0);
            int socket = s0->get_socket();
            manager.put(socket, s0);

            Socket* result = manager.get(socket);
            CHECK_EQUAL(s0->get_socket(), result->get_socket());
            result = manager.get(ap0);
            CHECK_EQUAL(s0->get_socket(), result->get_socket());
            CHECK(1 == manager.size());

            // Two
            Socket* s1 = new Socket(protocol, ap1);
            socket = s1->get_socket();
            manager.put(socket, s1);

            result = manager.get(socket);
            CHECK_EQUAL(s1->get_socket(), result->get_socket());
            result = manager.get(ap1);
            CHECK_EQUAL(s1->get_socket(), result->get_socket());
            CHECK(2 == manager.size());

            // Three
            Socket* s2 = new Socket(protocol, ap2);
            socket = s2->get_socket();
            manager.put(socket, s2);

            result = manager.get(socket);
            CHECK_EQUAL(s2->get_socket(), result->get_socket());
            result = manager.get(ap2);
            CHECK_EQUAL(s2->get_socket(), result->get_socket());
            CHECK(3 == manager.size());

            // Four
            Socket* s3 = new Socket(protocol, ap3);
            socket = s3->get_socket();
            manager.put(socket, s3);

            result = manager.get(socket);
            CHECK_EQUAL(s3->get_socket(), result->get_socket());
            result = manager.get(ap3);
            CHECK_EQUAL(s3->get_socket(), result->get_socket());
            CHECK(4 == manager.size());


            // Remove elements in reverse order
            // Remove the first one
            socket = s3->get_socket();
            manager.erase_at(s3->get_socket());
            CHECK(3 == manager.size());
            CHECK(manager.get(socket) == NULL);

            socket = s2->get_socket();
            manager.erase_at(s2->get_socket());
            CHECK(2 == manager.size());
            CHECK(manager.get(socket) == NULL);

            socket = s1->get_socket();
            manager.erase_at(s1->get_socket());
            CHECK(1 == manager.size());
            CHECK(manager.get(socket) == NULL);

            socket = s0->get_socket();
            manager.erase_at(s0->get_socket());
            CHECK(0 == manager.size());
            CHECK(manager.get(socket) == NULL);
            
        }
    }
}

#endif	/* _SOCKETMANAGERTEST_H */

