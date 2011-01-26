/* 
 * File:   SourceGetterTest.h
 * Author: rbuck
 *
 * Created on January 26, 2011, 10:48 AM
 */

#ifndef _SOURCEGETTERTEST_H
#define	_SOURCEGETTERTEST_H

#include "../headers/SourceGetter.h"

#include "UnitTest++.h"

#define sg SourceGetter::instance()

using namespace std;

namespace {

    SUITE(SourceGetter) {

        // TODO: These tests can only be run on ilab1
        // We should make it generic
        // Also, we need to test it on the mesh nodes (or another machine with more than one interface)

        TEST(SourceGetterLocalHost) {
            string destination = "127.0.0.1";
            string expected = "127.0.0.1";
            string result = sg.get_source_address(destination);
            CHECK_EQUAL(expected, result);
        }

        TEST(AddressResolve) {
            string expected = "192.168.21.101";
            for (int i = 1; i < 8; i++) {
                char buffer[1000];
                memset(buffer, 0, 1000);
                sprintf(buffer, "192.168.21.10%d", i);
                
                string destination = buffer;
                string result = sg.get_source_address(destination);
                CHECK_EQUAL(expected, result);
            }
        }

        TEST(AddressResolveCache) {
            // No way to ensure we hit the cache...
            string destination = "192.168.21.102";
            string expected = "192.168.21.101";
            string result = sg.get_source_address(destination);
            CHECK_EQUAL(expected, result);
        }


    }
}


#endif	/* _SOURCEGETTERTEST_H */

