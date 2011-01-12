/* 
 * File:   IntegrationTest.h
 * Author: rbuck
 *
 * Created on December 2, 2010, 9:03 AM
 */

#ifndef _INTEGRATIONTEST_H
#define	_INTEGRATIONTEST_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include "UnitTest++.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"
#include "../headers/AddressPort.h"
#include "../headers/GarbageCollector.h"
#include "../headers/Semaphore.h"

using namespace std;

namespace {

    void startup_backend() {
        int value = system("./wifu-end");
        if (value < 0) {
            perror("Error starting wifu-end");
        }
        // We have to sleep so we can ensure the back end is up and running
        sleep(1);
    }

    void kill_backend() {
        int value = system("killall wifu-end");
        if (value < 0) {
            perror("Error killing wifu-end");
        }
    }

    class BackEndFixture {
    public:

        BackEndFixture() {
            startup_backend();
        }

        ~BackEndFixture() {
            kill_backend();
        }
    };

    void socket_test() {
        // socket
        for (int i = 0; i < 100; i++) {
            // Check valid
            int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
            CHECK(socket >= 0);

            // Check invalid (i != SIMPLE_TCP)
            socket = wifu_socket(AF_INET, SOCK_STREAM, i);
            CHECK(socket == -1);
        }
    }

    void bind_test() {
        struct sockaddr_in to_bind;
        socklen_t length = sizeof (struct sockaddr_in);
        memset(&to_bind, 0, length);

        to_bind.sin_family = AF_INET;
        to_bind.sin_port = htons(5000);
        //TODO: do we want to ever bind on any other address?
        to_bind.sin_addr.s_addr = INADDR_ANY;

        // Bad fd
        //TODO: need to close all the sockets opened up above as we may collide
        // (badfd may actually have been created above)
        int badfd = 4000;
        int result = wifu_bind(badfd, (const struct sockaddr *) & to_bind, length);
        CHECK_EQUAL(EBADF, errno);
        CHECK(result < 0);

        // Should be successful
        int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        CHECK_EQUAL(0, result);


        // Should fail (already bound)
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        CHECK(result < 0);
        CHECK_EQUAL(EINVAL, errno);
    }

    void listen_test() {
        // Bad fd
        // TODO: need to close all the sockets opened up above as we may collide
        // (badfd may actually have been created above)
        int badfd = 4000;
        int queue_size = 5;
        int result = wifu_listen(badfd, queue_size);
        CHECK_EQUAL(EBADF, errno);
        CHECK(result < 0);

        struct sockaddr_in to_bind;
        socklen_t length = sizeof (struct sockaddr_in);
        memset(&to_bind, 0, length);
        to_bind.sin_family = AF_INET;
        to_bind.sin_port = htons(5001);
        to_bind.sin_addr.s_addr = INADDR_ANY;

        // Should be successful
        int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        CHECK_EQUAL(0, result);
        result = wifu_listen(socket, 5);
        CHECK_EQUAL(0, result);

        // Should fail
        result = wifu_listen(socket, 5);
        CHECK(result < 0);
        CHECK_EQUAL(EADDRINUSE, errno);
    }

    struct var {
        Semaphore* sem_;
        AddressPort* to_bind_;
    };

    void* thread(void* args) {

        struct var* v = (struct var*) args;
        AddressPort* to_bind = v->to_bind_;
        Semaphore* sem = v->sem_;
        cout << "In thread" << endl;

        // Create server
        int server = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof(struct sockaddr_in));
        CHECK_EQUAL(0, result);
        result = wifu_listen(server, 5);
        CHECK_EQUAL(0, result);
        
        cout << "Listening on: " << to_bind->to_s() << endl;
        sem->post();
    }

    void connect_test() {
        AddressPort to_connect("127.0.0.1", 5002);

        struct var v;
        v.sem_ = new Semaphore();
        v.sem_->init(0);
        v.to_bind_ = new AddressPort("127.0.0.1", 5002);

        pthread_t t;
        if(pthread_create(&t, NULL, &thread, &v) != 0) {
            perror("Error creating new thread in IntegrationTest.h");
            CHECK(false);
            return;
        }

        v.sem_->wait();

        cout << "Connecting to: " << to_connect.to_s() << endl;


        // Create client
        int client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        int result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof(struct sockaddr_in));
        CHECK_EQUAL(0, result);
    }

    SUITE(IntegrationTest) {

        TEST_FIXTURE(BackEndFixture, IntegrationTest) {

            socket_test();
            bind_test();
            listen_test();
            connect_test();
        }
    }
}


#endif	/* _INTEGRATIONTEST_H */

