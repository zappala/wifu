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
#include "../headers/Socket.h"
#include "events/TimeoutEvent.h"
#include "defines.h"
#include <pthread.h>
#include <sys/un.h>

using namespace std;

SUITE(PriorityQueue) {

    bool signalRaised;

    void enqueueHandler(int signal) {
        if (signal == SIG_ENQUEUE_EVENT)
            signalRaised = true;
    }

    TEST(enqueue) {
        PriorityQueue<Event*, EventComparator> priorityQueue;
        CHECK(priorityQueue.size() == 0);
        CHECK(priorityQueue.isEmpty() == true);

        Socket* s = new Socket(0,1,2);
        TimeoutEvent event1(s, 5, 0);
        TimeoutEvent event2(s, 10, 0);

        signalRaised == false;
        signal(SIG_ENQUEUE_EVENT, enqueueHandler);
        priorityQueue.enqueue(&event1, true);
        CHECK(signalRaised == true);

        priorityQueue.enqueue(&event2);
        CHECK(priorityQueue.size() == 2);
        CHECK(priorityQueue.isEmpty() == false);

        priorityQueue.enqueue(&event1);
        CHECK(priorityQueue.size() == 3);
    }

    void* enqueuer(void * priorityQueue) {
        Socket* s = new Socket(0,1,2);
        TimeoutEvent event1(s, 5, 0);
        PriorityQueue<Event*, EventComparator>* pQueue =
                (PriorityQueue<Event*, EventComparator>*) priorityQueue;
        signalRaised == false;
        usleep(100000);
        pQueue->enqueue(&event1, true);
    }

    TEST(dequeue) {
        pthread_t enqueueThread;
        PriorityQueue<Event*, EventComparator> priorityQueue;
        CHECK(priorityQueue.size() == 0);
        CHECK(priorityQueue.isEmpty() == true);

        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            CHECK(false);
        priorityQueue.dequeue();
        CHECK(priorityQueue.size() == 0);
        CHECK(priorityQueue.isEmpty() == true);
        CHECK(signalRaised == true);

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
        CHECK(priorityQueue.size() == 5);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event1);
        CHECK(priorityQueue.size() == 4);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event2);
        CHECK(priorityQueue.size() == 3);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event3);
        CHECK(priorityQueue.size() == 2);
        CHECK(priorityQueue.isEmpty() == false);

        priorityQueue.enqueue(&event2);
        CHECK(priorityQueue.size() == 3);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event2);
        CHECK(priorityQueue.size() == 2);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event4);
        CHECK(priorityQueue.size() == 1);
        CHECK(priorityQueue.isEmpty() == false);

        CHECK(priorityQueue.dequeue() == &event4);
        CHECK(priorityQueue.size() == 0);
        CHECK(priorityQueue.isEmpty() == true);

        if (pthread_create(&enqueueThread, NULL, &enqueuer, &priorityQueue) != 0)
            CHECK(false);
        priorityQueue.dequeue();
        CHECK(priorityQueue.size() == 0);
        CHECK(priorityQueue.isEmpty() == true);
        CHECK(signalRaised == true);
    }

}

#endif /* PRIORITYQUEUETEST_H_ */
