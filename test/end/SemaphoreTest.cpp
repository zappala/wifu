/* 
 * File:   SemaphoreTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 2:57 PM
 */

#ifndef _SEMAPHORETEST_H
#define	_SEMAPHORETEST_H

#include "Semaphore.h"
#include "Utils.h"
#include "defines.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace std;

namespace {

    TEST(SemaphoreTest, Init) {
        Semaphore s;
        s.init(1);
        int val = s.get_value();
        ASSERT_EQ(1, val);
    }

    TEST(SemaphoreTest, Post) {
        Semaphore s;
        s.init(1);
        int val = s.get_value();
        ASSERT_EQ(1, val);

        s.post();
        val = s.get_value();
        ASSERT_EQ(2, val);
    }

    TEST(SemaphoreTest, Wait) {
        Semaphore s;
        s.init(1);
        int val = s.get_value();
        ASSERT_EQ(1, val);

        s.wait();
        val = s.get_value();
        ASSERT_EQ(0, val);
    }

    TEST(SemaphoreTest, TryWait) {
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

    TEST(SemaphoreTest, TimedWait) {
        Semaphore s;
        s.init(1);
        int val = s.get_value();
        ASSERT_EQ(1, val);

        struct timespec timer;

        {
            Utils::get_timespec_future_time(0, 500000, &timer);
            bool timedout = s.timed_wait(&timer);
            ASSERT_TRUE(!timedout);
        }

        Utils::get_timespec_future_time(0, 500000, &timer);
        bool timedout = s.timed_wait(&timer);
        ASSERT_TRUE(timedout);
    }

    TEST(SemaphoreTest, GetValue) {
        Semaphore s;
        s.init(1);
        int val = s.get_value();
        ASSERT_EQ(1, val);
    }
}

#endif	/* _SEMAPHORETEST_H */

