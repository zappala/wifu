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

        TEST(SourceGetterLocalHost) {
            string destination = "127.0.0.1";
            string expected = "127.0.0.1";
            string result = sg.get_source_address(destination);
            CHECK_EQUAL(expected, result);
        }

        TEST(AddressResolve) {
            string destination = "192.168.21.102";
            string expected = "192.168.21.101";
            string result = sg.get_source_address(destination);
            CHECK_EQUAL(expected, result);
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

