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
        gcstring destination = "127.0.0.1";
        gcstring expected = "127.0.0.1";
        gcstring result = sg.get_source_address(destination);
        ASSERT_EQ(expected, result);
    }

    gcstring get_ip_address() {
        gcstring address;

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
        gcstring expected = get_ip_address();
        for (int i = 1; i < 8; i++) {
            char buffer[1000];
            memset(buffer, 0, 1000);
            sprintf(buffer, "192.168.21.10%d", i);

            gcstring destination = buffer;
            gcstring result = sg.get_source_address(destination);
            ASSERT_EQ(expected, result);
        }
    }

    TEST(SourceGetter, AddressResolveCache) {
        // No way to ensure we hit the cache...
        gcstring destination = "192.168.21.102";
        gcstring expected = get_ip_address();
        gcstring result = sg.get_source_address(destination);
        ASSERT_EQ(expected, result);
    }
}


#endif	/* _SOURCEGETTERTEST_H */

