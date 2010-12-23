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

#include "UnitTest++.h"
#include "../applib/wifu_socket.h"

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

    SUITE(IntegrationTest) {

        TEST_FIXTURE(BackEndFixture, Socket) {

            for (int i = 0; i < 1000; i++) {
                // Check valid
                int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
                CHECK(socket >= 0);

                // Check invalid (i != SIMPLE_TCP)
                socket = wifu_socket(AF_INET, SOCK_STREAM, i);
                CHECK(socket == -1);
            }



            
        }
    }
}


#endif	/* _INTEGRATIONTEST_H */

