/* 
 * File:   UDPSocketTest.h
 * Author: rbuck
 *
 * Created on November 15, 2010, 1:49 PM
 */

#ifndef _ADDRESSPORTTEST_H
#define	_ADDRESSPORTTEST_H

#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"

using namespace std;

namespace {

    SUITE(AddressPort) {

        TEST(AddressPortStringConstructor) {
            string address("127.0.0.1");
            int port = 5000;

            AddressPort ap(address, port);

            ASSERT_EQ(address, ap.get_address());
            ASSERT_EQ(port, ap.get_port());
        }

        TEST(AddressPortCharStarConstructor) {
            const char* address = "127.0.0.1";
            int port = 5000;

            AddressPort ap(address, port);

            ASSERT_EQ(string(address), ap.get_address());
            ASSERT_EQ(port, ap.get_port());
        }

        TEST(AddressPortSockaddrConstructor) {
            string address("127.0.0.1");
            int port = 5000;

            struct sockaddr_in addr;

            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            if (!inet_aton(address.c_str(), &addr.sin_addr)) {
                cout << "error converting ip address to binary" << endl;
                assert(false);
            }

            AddressPort ap(&addr);

            ASSERT_EQ(address, ap.get_address());
            ASSERT_EQ(port, ap.get_port());
        }

        TEST(AddressCopyConstructor) {
            string address("127.0.0.1");
            int port = 5000;

            AddressPort ap(address, port);
            AddressPort copy(ap);

            ASSERT_EQ(ap.get_address(), copy.get_address());
            ASSERT_EQ(ap.get_port(), copy.get_port());
            CHECK(ap.get_network_struct_ptr() != copy.get_network_struct_ptr());
        }

        TEST(AddressOperatorEqualsConstructor) {
            string address("127.0.0.1");
            int port = 5000;

            AddressPort ap(address, port);
            AddressPort copy(ap);

            CHECK(ap == copy);

            copy = AddressPort(address.c_str(), 5001);
            CHECK(!(ap == copy));

            address = "127.0.0.2";
            copy = AddressPort(address, 5000);
            CHECK(!(ap == copy));
        }

        TEST(AddressOperatorNotEqualsConstructor) {
            string address("127.0.0.1");
            int port = 5000;

            AddressPort ap(address, port);
            AddressPort copy(ap);

            CHECK(!(ap != copy));

            copy = AddressPort(address.c_str(), 5001);
            CHECK(ap != copy);

            address = "127.0.0.2";
            copy = AddressPort(address, 5000);
            CHECK(ap != copy);
        }

        TEST(AddressToString) {

            string expected = "Address: 127.0.0.1 Port: 5000";

            string address("127.0.0.1");
            int port = 5000;

            AddressPort ap(address, port);
            ASSERT_EQ(expected, ap.to_s());
        }
    }
}


#endif	/* _ADDRESSPORTTEST_H */


