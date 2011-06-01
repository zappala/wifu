/*
 * PriorityQueueTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef PRIORITYQUEUETEST_H_
#define PRIORITYQUEUETEST_H_

#include "gtest/gtest.h"
#include "PriorityQueue.h"
#include "TimeoutEventComparator.h"
#include "Socket.h"
#include "events/framework_events/TimeoutEvent.h"
#include "defines.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include <pthread.h>
#include <sys/un.h>
#include "PriorityEventComparator.h"

using namespace std;

namespace {

    bool signalRaised;

    void enqueueHandler(int signal) {
        if (signal == SIG_ENQUEUE_EVENT)
            signalRaised = true;
    }

    TEST(PriorityQueueTest, enqueue) {
        PriorityQueue<Event*, TimeoutEventComparator> priorityQueue;
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);

        Socket* s = new Socket(0, 1, 2);
        TimeoutEvent event1(s, 5, 0);
        TimeoutEvent event2(s, 10, 0);

        signalRaised = false;
        signal(SIG_ENQUEUE_EVENT, enqueueHandler);
        priorityQueue.enqueue(&event1, true);
        ASSERT_TRUE(signalRaised == true);

        priorityQueue.enqueue(&event2);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        priorityQueue.enqueue(&event1);
        ASSERT_TRUE(priorityQueue.size() == 3);
    }

    void* enqueuer(void* priorityQueue) {
        Socket* s = new Socket(0, 1, 2);
        TimeoutEvent* event1 = new TimeoutEvent(s, 5, 0);
        PriorityQueue<Event*, TimeoutEventComparator>* pQueue =
                (PriorityQueue<Event*, TimeoutEventComparator>*) priorityQueue;
        signalRaised = false;
        usleep(100000);
        pQueue->enqueue(event1, true);
    }

    TEST(PriorityQueueTest, dequeue) {
        pthread_t enqueueThread;
        PriorityQueue<Event*, TimeoutEventComparator> priorityQueue;
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);

        signal(SIG_ENQUEUE_EVENT, enqueueHandler);
        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            FAIL();
        priorityQueue.dequeue();
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);
        ASSERT_TRUE(signalRaised == true);

        Socket* s = new Socket(0, 1, 2);
        TimeoutEvent event1(s, 0, 0);
        TimeoutEvent event2(s, 5, 0);
        TimeoutEvent event3(s, 10, 100);
        TimeoutEvent event4(s, 15, 0);

        priorityQueue.enqueue(&event3);
        priorityQueue.enqueue(&event2);
        priorityQueue.enqueue(&event4);
        priorityQueue.enqueue(&event1);
        priorityQueue.enqueue(&event4);
        ASSERT_TRUE(priorityQueue.size() == 5);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event1);
        ASSERT_TRUE(priorityQueue.size() == 4);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event2);
        ASSERT_TRUE(priorityQueue.size() == 3);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event3);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        priorityQueue.enqueue(&event2);
        ASSERT_TRUE(priorityQueue.size() == 3);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event2);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event4);
        ASSERT_TRUE(priorityQueue.size() == 1);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event4);
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);

        pthread_cancel(enqueueThread);
        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            FAIL();
        priorityQueue.dequeue();
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);
        ASSERT_TRUE(signalRaised == true);
        pthread_cancel(enqueueThread);
    }

    TEST(PriorityQueueTest, clear) {
        PriorityQueue<Event*, TimeoutEventComparator> priorityQueue;
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);

        Socket* s = new Socket(0, 1, 2);
        TimeoutEvent event1(s, 0, 0);
        TimeoutEvent event2(s, 5, 0);
        TimeoutEvent event3(s, 10, 100);
        TimeoutEvent event4(s, 15, 0);

        priorityQueue.enqueue(&event3);
        priorityQueue.enqueue(&event2);
        priorityQueue.enqueue(&event4);
        priorityQueue.enqueue(&event1);
        priorityQueue.enqueue(&event4);
        ASSERT_TRUE(priorityQueue.size() == 5);
        ASSERT_TRUE(priorityQueue.is_empty() == false);

        priorityQueue.clear();
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.is_empty() == true);
    }

    class IntComparator {
    public:

        bool operator()(int const& t1, int const& t2) {
            return t1 < t2;
        }

    };

    TEST(PriorityQueueTest, top) {
        PriorityQueue<int, IntComparator> q;
        int expected = 5;
        q.enqueue(6);
        q.enqueue(expected);

        int actual = q.top();

        ASSERT_EQ(expected, actual);
        ASSERT_FALSE(q.is_empty());
        ASSERT_EQ(2, q.size());

        actual = 0;
        actual = q.dequeue();

        ASSERT_EQ(expected, actual);
        ASSERT_FALSE(q.is_empty());
        ASSERT_EQ(1, q.size());

        actual = q.top();
        expected = 6;

        ASSERT_EQ(expected, actual);
        ASSERT_FALSE(q.is_empty());
        ASSERT_EQ(1, q.size());

        actual = 0;
        actual = q.dequeue();

        ASSERT_EQ(expected, actual);
        ASSERT_TRUE(q.is_empty());
        ASSERT_EQ(0, q.size());
    }

    TEST(PriorityQueueTest, FIFO) {
        PriorityQueue<Event*, TimeoutEventComparator> q_;

        Socket* s = new Socket(0, 1, 2);
        list<Event*> a, b, c;


        // high priority
        for (int i = 0; i < 10; i++) {
            TimeoutEvent* e = new TimeoutEvent(s, 1, 0);
            q_.enqueue(e);
            a.push_back(e);
        }

        // low priority
        for (int i = 0; i < 10; i++) {
            TimeoutEvent* e = new TimeoutEvent(s, 10, 0);
            q_.enqueue(e);
            b.push_back(e);
        }

        // high priority
        for (int i = 0; i < 10; i++) {
            TimeoutEvent* e = new TimeoutEvent(s, 1, 0);
            q_.enqueue(e);
            c.push_back(e);
        }

        // should get a, c, b
        list<Event*> all;
        all.insert(all.end(), a.begin(), a.end());
        all.insert(all.end(), c.begin(), c.end());
        all.insert(all.end(), b.begin(), b.end());

        while (!all.empty()) {
            Event* actual = q_.dequeue();
            Event* expected = all.front();
            all.pop_front();

            ASSERT_EQ(expected, actual);
        }
    }

    TEST(PriorityQueueTest, A) {
        PriorityQueue<Event*, PriorityEventComparator> q_;
        Socket* s = new Socket(0, 0, 0);
        WiFuPacket* p = new WiFuPacket;

        Event* a = new ConnectionInitiatedEvent(s,s);
        Event* b = new SendPacketEvent(s, p);

        ASSERT_EQ(HIGH, a->get_priority());
        ASSERT_EQ(HIGH, b->get_priority());

        q_.enqueue(a);
        q_.enqueue(b);

        EXPECT_EQ(a, q_.dequeue());
        EXPECT_EQ(b, q_.dequeue());
    }
}

#endif /* PRIORITYQUEUETEST_H_ */
