/* 
 * File:   SemaphoreTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 2:57 PM
 */

#ifndef _SEMAPHORETEST_H
#define	_SEMAPHORETEST_H

#include "../headers/Semaphore.h"
#include "../headers/defines.h"

#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"

using namespace std;

namespace {

    void get_timer(int nanoseconds, struct timespec* timer) {
        clock_gettime(CLOCK_REALTIME, timer);

        timer->tv_nsec += nanoseconds;

        while (timer->tv_nsec >= NANOSECONDS_IN_SECONDS) {
            timer->tv_sec += 1;
            timer->tv_nsec -= NANOSECONDS_IN_SECONDS;
        }
    }

    SUITE(Semaphore) {

        TEST(SemaphoreConstructor) {
            Semaphore s;
            int val = s.get_value();
            CHECK_EQUAL(0, val);
        }

        TEST(SemaphoreInit) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);
        }

        TEST(SemaphorePost) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);

            s.post();
            val = s.get_value();
            CHECK_EQUAL(2, val);
        }

        TEST(SemaphoreWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);

            s.wait();
            val = s.get_value();
            CHECK_EQUAL(0, val);
        }

        TEST(SemaphoreTryWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);

            s.try_wait();
            val = s.get_value();
            CHECK_EQUAL(0, val);

            s.try_wait();
            val = s.get_value();
            CHECK_EQUAL(0, val);
        }

        TEST(SemaphoreTimedWait) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);

            struct timespec timer;

            {
                get_timer(500000, &timer);
                UNITTEST_TIME_CONSTRAINT(50);
                bool timedout = s.timed_wait(&timer);
                CHECK(!timedout);
            }

            get_timer(500000, &timer);
            bool timedout = s.timed_wait(&timer);
            CHECK(timedout);
        }

        TEST(SemaphoreGetValue) {
            Semaphore s;
            s.init(1);
            int val = s.get_value();
            CHECK_EQUAL(1, val);
        }
    }
}



#endif	/* _SEMAPHORETEST_H */

