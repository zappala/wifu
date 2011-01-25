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

#include "gtest/gtest.h"
#include "../headers/Queue.h"
#include "../headers/IQueue.h"

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
		ASSERT_TRUE(!q->isEmpty());
		ASSERT_EQ(value, q->dequeue());
		ASSERT_EQ(0, q->size());
		ASSERT_TRUE(q->isEmpty());

		signaled = false;
		q->enqueue(value, true);

		ASSERT_TRUE(signaled);
		ASSERT_EQ(1, q->size());
		ASSERT_TRUE(!q->isEmpty());
		ASSERT_EQ(value, q->dequeue());
		ASSERT_EQ(0, q->size());
		ASSERT_TRUE(q->isEmpty());

		delete q;
	}
}

#endif	/* _QUEUETEST_H */

