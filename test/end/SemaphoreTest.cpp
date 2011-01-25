/* 
 * File:   SemaphoreTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 2:57 PM
 */

#ifndef _SEMAPHORETEST_H
#define	_SEMAPHORETEST_H

#include "../headers/Semaphore.h"
#include "../headers/Utils.h"
#include "../headers/defines.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

namespace {

    SUITE(Semaphore) {

        TEST(SemaphoreConstructor) {
            Semaphore s;
            int val = s.get_value();
            ASSERT_EQ(0, val);
        }

        TEST(SemaphoreInit) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);
        }

        TEST(SemaphorePost) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);

            s.post();
            val = s.get_value();
            ASSERT_EQ(2, val);
        }

        TEST(SemaphoreWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);

            s.wait();
            val = s.get_value();
            ASSERT_EQ(0, val);
        }

        TEST(SemaphoreTryWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);

            s.try_wait();
            val = s.get_value();
            ASSERT_EQ(0, val);

            s.try_wait();
            val = s.get_value();
            ASSERT_EQ(0, val);
        }

        TEST(SemaphoreTimedWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);

            struct timespec timer;

            {
                Utils::get_timespec_future_time(0, 500000, &timer);
                UNITTEST_TIME_CONSTRAINT(50);
                bool timedout = s.timed_wait(&timer);
                ASSERT_TRUE(!timedout);
            }

            Utils::get_timespec_future_time(0, 500000, &timer);
            bool timedout = s.timed_wait(&timer);
            ASSERT_TRUE(timedout);
        }

        TEST(SemaphoreGetValue) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            ASSERT_EQ(1, val);
        }
    }
}



#endif	/* _SEMAPHORETEST_H */

