/* 
 * File:   SourceGetterTest.h
 * Author: rbuck
 *
 * Created on January 26, 2011, 10:48 AM
 */

#ifndef _SOURCEGETTERTEST_H
#define	_SOURCEGETTERTEST_H

#include "SourceGetter.h"

#include "gtest/gtest.h"

#define sg SourceGetter::instance()

using namespace std;

namespace {

    // TODO: These tests can only be run on the ilab machine
    // We should make it generic
    // Also, we need to test it on the mesh nodes (or another machine with more than one interface)

    TEST(SourceGetter, LocalHost) {
        string destination = "127.0.0.1";
        string expected = "127.0.0.1";
        string result = sg.get_source_address(destination);
        ASSERT_EQ(expected, result);
    }

    string get_ip_address() {
        string address;

        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s <= 0) {
            perror("SourceGetterTest::get_ip_address(): socket()");
            exit(EXIT_FAILURE);
        }

        struct ifreq ifr;
        strcpy(ifr.ifr_name, "eth0");

        if (!ioctl(s, SIOCGIFADDR, &ifr)) {
            address = inet_ntoa(((struct sockaddr_in *) & ifr.ifr_addr)->sin_addr);
        }
        close(s);
        return address;
    }

    TEST(SourceGetter, AddressResolve) {
        string expected = get_ip_address();
        for (int i = 1; i < 8; i++) {
            char buffer[1000];
            memset(buffer, 0, 1000);
            sprintf(buffer, "192.168.21.10%d", i);

            string destination = buffer;
            string result = sg.get_source_address(destination);
            ASSERT_EQ(expected, result);
        }
    }

    TEST(SourceGetter, AddressResolveCache) {
        // No way to ensure we hit the cache...
        string destination = "192.168.21.102";
        string expected = get_ip_address();
        string result = sg.get_source_address(destination);
        ASSERT_EQ(expected, result);
    }
}


#endif	/* _SOURCEGETTERTEST_H */

