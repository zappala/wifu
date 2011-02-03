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
#include "Socket.h"
#include "events/TimeoutEvent.h"
#include "defines.h"
#include <pthread.h>
#include <sys/un.h>

using namespace std;

namespace {

    bool signalRaised;

    void enqueueHandler(int signal) {
        if (signal == SIG_ENQUEUE_EVENT)
            signalRaised = true;
    }

    TEST(PriorityQueueTest, enqueue) {
        PriorityQueue<Event*, EventComparator> priorityQueue;
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.isEmpty() == true);

        Socket* s = new Socket(0,1,2);
        TimeoutEvent event1(s, 5, 0);
        TimeoutEvent event2(s, 10, 0);

        signalRaised == false;
        signal(SIG_ENQUEUE_EVENT, enqueueHandler);
        priorityQueue.enqueue(&event1, true);
        ASSERT_TRUE(signalRaised == true);

        priorityQueue.enqueue(&event2);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        priorityQueue.enqueue(&event1);
        ASSERT_TRUE(priorityQueue.size() == 3);
    }

    void* enqueuer(void * priorityQueue) {
        Socket* s = new Socket(0,1,2);
        TimeoutEvent* event1 = new TimeoutEvent(s, 5, 0);
        PriorityQueue<Event*, EventComparator>* pQueue =
                (PriorityQueue<Event*, EventComparator>*) priorityQueue;
        signalRaised == false;
        usleep(100000);
        pQueue->enqueue(event1, true);
    }

    TEST(PriorityQueueTest, dequeue) {
        pthread_t enqueueThread;
        PriorityQueue<Event*, EventComparator> priorityQueue;
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.isEmpty() == true);

        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            ASSERT_TRUE(false);
        priorityQueue.dequeue();
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.isEmpty() == true);
        ASSERT_TRUE(signalRaised == true);

        Socket* s = new Socket(0,1,2);
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
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event1);
        ASSERT_TRUE(priorityQueue.size() == 4);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event2);
        ASSERT_TRUE(priorityQueue.size() == 3);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event3);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        priorityQueue.enqueue(&event2);
        ASSERT_TRUE(priorityQueue.size() == 3);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event2);
        ASSERT_TRUE(priorityQueue.size() == 2);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event4);
        ASSERT_TRUE(priorityQueue.size() == 1);
        ASSERT_TRUE(priorityQueue.isEmpty() == false);

        ASSERT_TRUE(priorityQueue.dequeue() == &event4);
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.isEmpty() == true);

        pthread_cancel(enqueueThread);
        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            ASSERT_TRUE(false);
        priorityQueue.dequeue();
        ASSERT_TRUE(priorityQueue.size() == 0);
        ASSERT_TRUE(priorityQueue.isEmpty() == true);
        ASSERT_TRUE(signalRaised == true);
        pthread_cancel(enqueueThread);
    }

}

#endif /* PRIORITYQUEUETEST_H_ */
