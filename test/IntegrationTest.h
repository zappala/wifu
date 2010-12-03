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

#include "headers/UnitTest++.h"
#include "../applib/wifu_socket.h"

using namespace std;

namespace {

    void startup_backend() {
        int value = system("./wifu-end");
        if (value < 0) {
            perror("Error starting wifu-end");
        }
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
            int socket = wifu_socket(AF_INET, SOCK_STREAM, 0);
            cout << "Socket Returned: " <<socket << endl;
        }
    }
}


#endif	/* _INTEGRATIONTEST_H */

