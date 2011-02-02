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

#include "gtest/gtest.h"
#include "AddressPort.h"

using namespace std;

namespace {

    TEST(AddpressPortTest, StringConstructor) {
        string address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);

        ASSERT_EQ(address, ap.get_address());
        ASSERT_EQ(port, ap.get_port());
    }

    TEST(AddpressPortTest, CharStarConstructor) {
        const char* address = "127.0.0.1";
        int port = 5000;

        AddressPort ap(address, port);

        ASSERT_EQ(string(address), ap.get_address());
        ASSERT_EQ(port, ap.get_port());
    }

    TEST(AddpressPortTest, SockaddrConstructor) {
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

    TEST(AddpressPortTest, CopyConstructor) {
        string address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        AddressPort copy(ap);

        ASSERT_EQ(ap.get_address(), copy.get_address());
        ASSERT_EQ(ap.get_port(), copy.get_port());
        ASSERT_TRUE(ap.get_network_struct_ptr() != copy.get_network_struct_ptr());
    }

    TEST(AddpressPortTest, OperatorEqualsConstructor) {
        string address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        AddressPort copy(ap);

        ASSERT_TRUE(ap == copy);

        copy = AddressPort(address.c_str(), 5001);
        ASSERT_TRUE(!(ap == copy));

        address = "127.0.0.2";
        copy = AddressPort(address, 5000);
        ASSERT_TRUE(!(ap == copy));
    }

    TEST(AddpressPortTest, OperatorNotEqualsConstructor) {
        string address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        AddressPort copy(ap);

        ASSERT_TRUE(!(ap != copy));

        copy = AddressPort(address.c_str(), 5001);
        ASSERT_TRUE(ap != copy);

        address = "127.0.0.2";
        copy = AddressPort(address, 5000);
        ASSERT_TRUE(ap != copy);
    }

    TEST(AddpressPortTest, ToString) {

        string expected = "Address: 127.0.0.1 Port: 5000";

        string address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        ASSERT_EQ(expected, ap.to_s());
    }
}


#endif	/* _ADDRESSPORTTEST_H */


