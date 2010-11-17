/* 
 * File:   BinarySemaphoreTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 3:37 PM
 */

#ifndef _BINARYSEMAPHORETEST_H
#define	_BINARYSEMAPHORETEST_H

#include "../headers/BinarySemaphore.h"

#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"

using namespace std;

namespace {

    SUITE(BinarySemaphore) {

        TEST(BinarySemaphoreConstructorTest) {
            BinarySemaphore bs;
            CHECK_EQUAL(0, bs.get_value());
        }

        TEST(BinarySemaphoreInitTest) {
            BinarySemaphore bs;
            CHECK_EQUAL(0, bs.get_value());

            bs.init(0);
            CHECK_EQUAL(0, bs.get_value());

            BinarySemaphore bs1;
            bs1.init(1);
            CHECK_EQUAL(1, bs1.get_value());

            BinarySemaphore bs2;
            bs2.init(2);
            CHECK_EQUAL(1, bs2.get_value());
        }

        TEST(BinarySemaphorePostTest) {
            BinarySemaphore bs;

            bs.init(0);
            CHECK_EQUAL(0, bs.get_value());

            bs.post();
            CHECK_EQUAL(1, bs.get_value());

            bs.post();
            CHECK_EQUAL(1, bs.get_value());
        }
    }
}


#endif	/* _BINARYSEMAPHORETEST_H */

