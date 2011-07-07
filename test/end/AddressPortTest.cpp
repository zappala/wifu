/* 
 * File:   UDPSocketTest.h
 * Author: rbuck
 *
 * Created on November 15, 2010, 1:49 PM
 */

#ifndef _ADDRESSPORTTEST_H
#define	_ADDRESSPORTTEST_H

#include <iostream>

#include "gtest/gtest.h"
#include "AddressPort.h"
#include "exceptions/InvalidAddressException.h"

using namespace std;

namespace {

    TEST(AddressPort, StringConstructor) {
        gcstring address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);

        ASSERT_EQ(address, ap.get_address());
        ASSERT_EQ(port, ap.get_port());
    }

    TEST(AddressPort, CharStarConstructor) {
        const char* address = "127.0.0.1";
        int port = 5000;

        AddressPort ap(address, port);

        ASSERT_EQ(gcstring(address), ap.get_address());
        ASSERT_EQ(port, ap.get_port());

        ASSERT_THROW(AddressPort("", 0), InvalidAddressException);
    }

    TEST(AddressPort, SockaddrConstructor) {
        gcstring address("127.0.0.1");
        int port = 5000;

        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (!inet_aton(address.c_str(), &addr.sin_addr))
            FAIL() << "error converting ip address to binary";

        AddressPort ap(&addr);

        ASSERT_EQ(address, ap.get_address());
        ASSERT_EQ(port, ap.get_port());
    }

    TEST(AddressPort, CopyConstructor) {
        gcstring address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        AddressPort copy(ap);

        ASSERT_EQ(ap.get_address(), copy.get_address());
        ASSERT_EQ(ap.get_port(), copy.get_port());
        ASSERT_TRUE(ap.get_network_struct_ptr() != copy.get_network_struct_ptr());
    }

    TEST(AddressPort, AssignmentOperator) {
    	gcstring address("1.1.1.1");
    	int port = 1;

    	AddressPort one(address, port);
    	ASSERT_EQ(one.get_address(), address);
    	ASSERT_EQ(one.get_port(), port);

        one = one;
        ASSERT_EQ(one.get_address(), address);
        ASSERT_EQ(one.get_port(), port);
    }

    TEST(AddressPort, OperatorEqualsConstructor) {
        gcstring address("127.0.0.1");
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

    TEST(AddressPort, OperatorNotEqualsConstructor) {
        gcstring address("127.0.0.1");
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

    TEST(AddressPort, ToString) {

        gcstring expected = "Address: 127.0.0.1 Port: 5000";

        gcstring address("127.0.0.1");
        int port = 5000;

        AddressPort ap(address, port);
        ASSERT_EQ(expected, ap.to_s());
    }
}


#endif	/* _ADDRESSPORTTEST_H */


