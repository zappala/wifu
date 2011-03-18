#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"
#include "../headers/AddressPort.h"
#include "../headers/GarbageCollector.h"
#include "../headers/Semaphore.h"
#include "Timer.h"
#include "../headers/RandomStringGenerator.h"

#include "../headers/BackEndTest.h"

void* thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

    // Create server
    int server = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    EXPECT_EQ(0, result);
    result = wifu_listen(server, 5);
    EXPECT_EQ(0, result);
    sem->post();

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    AddressPort ap(&addr);
    string address("127.0.0.1");
    string res = ap.get_address();
    EXPECT_EQ(address, res);
    cout << "Connected to: " << ap.to_s() << endl;

}

void connect_test(int count) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t[count];
    struct var v[count];
    Timer timer[count];
    int client[count];
    int result[count];

    for (int i = 0; i < count; i++) {

        v[i].sem_ = new Semaphore();
        v[i].sem_->init(0);
        v[i].to_bind_ = new AddressPort("127.0.0.1", 5002);


        if (pthread_create(&(t[i]), NULL, &thread, &(v[i])) != 0)
            FAIL() << "Error creating new thread in IntegrationTest.connectTest";

        v[i].sem_->wait();

        // Make sure that the thread is in the accept state
        usleep(50000);

        // Create client

        timer[i].start();
        client[i] = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result[i] = wifu_connect(client[i], (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
        timer[i].stop();
        ASSERT_EQ(0, result[i]);

        cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer[i].get_duration_microseconds() << endl;
    }

    for(int i = 0; i < count; i++) {
        pthread_cancel(t[i]);
    }
}

TEST_F(BackEndTest, connectTest) {
    connect_test(1);

    // so we can see if we are doing something incorrect that would otherwise
    // be covered up by the exiting of this method
    sleep(5);
}

TEST_F(BackEndMockTestDropNone, mockConnectTest) {
    connect_test(1);

    // so we can see if we are doing something incorrect that would otherwise
    // be covered up by the exiting of this method
    sleep(5);
}