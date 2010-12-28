/* 
 * File:   QueueTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 4:05 PM
 */

#ifndef _QUEUETEST_H
#define	_QUEUETEST_H

#include <iostream>
#include <string>
#include <vector>
#include <signal.h>

#include "UnitTest++.h"
#include "../headers/Queue.h"
#include "../headers/IQueue.h"

using namespace std;

bool signaled;

namespace {

    SUITE(QueueTest) {

        void signal_handler(int signal) {
            CHECK_EQUAL(SIG_ENQUEUE_EVENT, signal);
            signaled = true;
        }

        TEST(QueueTest) {

            signal(SIG_ENQUEUE_EVENT, signal_handler);
            
            IQueue<int>* q = new Queue<int>();
            int value = 0;

            signaled = false;
            q->enqueue(value);

            CHECK(!signaled);
            CHECK_EQUAL(1, q->size());
            CHECK(!q->isEmpty());
            CHECK_EQUAL(value, q->dequeue());
            CHECK_EQUAL(0, q->size());
            CHECK(q->isEmpty());

            signaled = false;
            q->enqueue(value, true);

            CHECK(signaled);
            CHECK_EQUAL(1, q->size());
            CHECK(!q->isEmpty());
            CHECK_EQUAL(value, q->dequeue());
            CHECK_EQUAL(0, q->size());
            CHECK(q->isEmpty());

            delete q;
        }

    }
}

#endif	/* _QUEUETEST_H */

