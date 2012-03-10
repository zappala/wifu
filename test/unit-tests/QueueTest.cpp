/* 
 * File:   QueueTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 4:05 PM
 */

#ifndef _QUEUETEST_H
#define	_QUEUETEST_H

#include <iostream>
#include <signal.h>

#include "gtest/gtest.h"
#include "Queue.h"
#include "IQueue.h"

using namespace std;

bool signaled;

namespace {

    void signal_handler(int signal) {
        ASSERT_EQ(SIG_ENQUEUE_EVENT, signal);
        signaled = true;
    }

    TEST(QueueTest, all) {

        signal(SIG_ENQUEUE_EVENT, signal_handler);

        IQueue<int>* q = new Queue<int>();
        int value = 0;

        signaled = false;
        q->enqueue(value);

        ASSERT_TRUE(!signaled);
        ASSERT_EQ(1, q->size());
        ASSERT_TRUE(!q->is_empty());
        ASSERT_EQ(value, q->dequeue());
        ASSERT_EQ(0, q->size());
        ASSERT_TRUE(q->is_empty());

        signaled = false;
        q->enqueue(value, true);

        ASSERT_TRUE(signaled);
        ASSERT_EQ(1, q->size());
        ASSERT_TRUE(!q->is_empty());
        ASSERT_EQ(value, q->dequeue());
        ASSERT_EQ(0, q->size());
        ASSERT_TRUE(q->is_empty());

        delete q;
    }

    TEST(QueueTest, top) {
        Queue<int> q;
        int expected = 5;
        q.enqueue(expected);

        int actual = q.top();

        ASSERT_EQ(expected, actual);
        ASSERT_FALSE(q.is_empty());
        ASSERT_EQ(1, q.size());

        actual = 0;
        actual = q.dequeue();

        ASSERT_EQ(expected, actual);
        ASSERT_TRUE(q.is_empty());
        ASSERT_EQ(0, q.size());
    }
}

#endif	/* _QUEUETEST_H */

