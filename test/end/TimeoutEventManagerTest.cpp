/*
 * TimeoutEventManagerTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef TIMEOUTEVENTMANAGERTEST_H_
#define TIMEOUTEVENTMANAGERTEST_H_

#include "gtest/gtest.h"
#include "TimeoutEventManager.h"
#include "events/Event.h"
#include "events/TimeoutEvent.h"
#include "events/TimerFiredEvent.h"

#include <vector>
#include <time.h>
#include <algorithm>

#define dispatcher Dispatcher::instance()
#define timer TimeoutEventManager::instance()


using namespace std;

namespace {

    class TimeoutEventManagerHelper : public Module {
    private:

        Semaphore* sem;

        TimeoutEventManagerHelper() {
            reset();
        }

        IQueue<Event*>* q_;

    public:

        static TimeoutEventManagerHelper& instance() {
            static TimeoutEventManagerHelper instance_;
            return instance_;
        }

        void timer_fired(Event* e) {
            q_->enqueue(e);
            sem->post();
        }

        Semaphore* get_sem() {
            return sem;
        }

        void reset() {
            sem = new Semaphore();
            sem->init(0);
            q_ = new Queue<Event*>();
        }

        IQueue<Event*>* get_queue() {
            return q_;
        }


    };

#define helper TimeoutEventManagerHelper::instance()

    class TimeoutEventManagerTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            helper.reset();
            dispatcher.reset();
            dispatcher.map_event(type_name(TimeoutEvent), &timer);
            dispatcher.map_event(type_name(CancelTimerEvent), &timer);
            dispatcher.map_event(type_name(TimerFiredEvent), &TimeoutEventManagerHelper::instance());
            dispatcher.start_processing();
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TimeoutEventManagerTest, EnqueueOneDirectly) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        int nano = 10000000;
        TimeoutEvent* expected = new TimeoutEvent(s, seconds, nano);
        timer.enqueue(expected);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
        bool timedout = helper.get_sem()->timed_wait(&t);
        EXPECT_FALSE(timedout);

        TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
        TimeoutEvent* actual = event->get_timeout_event();
        EXPECT_EQ(expected, actual);

    }

    TEST_F(TimeoutEventManagerTest, DispatcherEnqueueOne) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        int nano = 10000000;
        TimeoutEvent* expected = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(expected);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
        bool timedout = helper.get_sem()->timed_wait(&t);
        EXPECT_FALSE(timedout);

        TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
        TimeoutEvent* actual = event->get_timeout_event();
        EXPECT_EQ(expected, actual);
    }

    void enqueue_two_helper(int seconds, long int nano, Event* A, Event* B) {
        struct timespec t;

        { // Ensure A occurs first
            Utils::get_timespec_future_time(seconds, nano * 2, &t);

            // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
            bool timedout = helper.get_sem()->timed_wait(&t);
            EXPECT_FALSE(timedout);

            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            TimeoutEvent* actual = event->get_timeout_event();
            EXPECT_EQ(A, actual);
        }

        { // Ensure B occurs second
            Utils::get_timespec_future_time(seconds, nano * 4, &t);

            // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
            bool timedout = helper.get_sem()->timed_wait(&t);
            EXPECT_FALSE(timedout);

            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            TimeoutEvent* actual = event->get_timeout_event();
            EXPECT_EQ(B, actual);
        }
    }

    TEST_F(TimeoutEventManagerTest, EnqueueTwoInOrder) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        long int nano = 1000000;
        TimeoutEvent* A = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(A);

        TimeoutEvent* B = new TimeoutEvent(s, seconds, nano * 2);
        dispatcher.enqueue(B);

        enqueue_two_helper(seconds, nano, A, B);
    }

    TEST_F(TimeoutEventManagerTest, EnqueueTwoOutOfOrder) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        long int nano = 1000000;

        TimeoutEvent* A = new TimeoutEvent(s, seconds, nano);
        TimeoutEvent* B = new TimeoutEvent(s, seconds, nano * 2);

        dispatcher.enqueue(B);
        dispatcher.enqueue(A);

        enqueue_two_helper(seconds, nano, A, B);
    }

    TEST_F(TimeoutEventManagerTest, MassEnqueueInOrder) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        long int nano = 100;
        int max = 10000;
        Event * events[max];

        for (int i = 0; i < max; ++i) {
            long int time = (nano * i) + nano;
            TimeoutEvent* e = new TimeoutEvent(s, seconds, time);
            events[i] = e;
            dispatcher.enqueue(e);
        }

        for (int i = 0; i < max; ++i) {
            Event* expected = events[i];
            helper.get_sem()->wait();
            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            Event* actual = event->get_timeout_event();
            EXPECT_EQ(expected, actual);
        }
    }

    TEST_F(TimeoutEventManagerTest, MassEnqueueReverseOrder) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 1;
        long int nano = 1000;
        int max = 10000;
        vector<Event*> events;

        for (int i = 0; i < max; ++i) {
            long int time = (nano * i) + nano;
            TimeoutEvent* e = new TimeoutEvent(s, seconds, time);
            events.push_back(e);
        }

        reverse(events.begin(), events.end());

        for (int i = 0; i < max; ++i) {
            TimeoutEvent* e = (TimeoutEvent*) events.at(i);
            dispatcher.enqueue(e);
        }

        reverse(events.begin(), events.end());

        for (int i = 0; i < max; ++i) {
            Event* expected = events.at(i);
            helper.get_sem()->wait();
            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            Event* actual = event->get_timeout_event();
            EXPECT_EQ(expected, actual);
        }
    }

    int get_random(int max) {
        srand ( time(NULL) );
        return rand() % max;
    }

    TEST_F(TimeoutEventManagerTest, MassEnqueueRandomOrder) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 1;
        long int nano = 1000;
        int max = 10000;
        vector<Event*> events;

        for (int i = 0; i < max; ++i) {
            long int time = (nano * i) + nano;
            TimeoutEvent* e = new TimeoutEvent(s, seconds, time);
            events.push_back(e);
        }

        vector<Event*> events_copy(events);

        while(!events.empty()) {
            int random = get_random(events.size());
            Event* e = events.at(random);
            events.erase(events.begin() + random);
            dispatcher.enqueue(e);
        }

        for (int i = 0; i < max; ++i) {
            Event* expected = events_copy.at(i);
            helper.get_sem()->wait();
            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            Event* actual = event->get_timeout_event();
            EXPECT_EQ(expected, actual);
        }
    }

    TEST_F(TimeoutEventManagerTest, CancelTimer) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        int nano = 10000000;
        TimeoutEvent* e = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(e);

        CancelTimerEvent* cancel_event = new CancelTimerEvent(e);
        dispatcher.enqueue(cancel_event);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 10, &t);

        bool timedout = helper.get_sem()->timed_wait(&t);
        EXPECT_TRUE(timedout);

        EXPECT_TRUE(helper.get_queue()->size() == 0);
    }

    TEST_F(TimeoutEventManagerTest, CancelTimer2) {
        int max = 5;
        int to_save = max - (max / 2);
        assert(to_save < max);
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;

        Event * events[max];

        // 1 ms
        long int nano = 1000000;

        TimeoutEvent* saved;

        // create max timers, 10 ms apart
        for (int i = 0; i < max; ++i) {
            int temp = (nano * i) + nano;

            TimeoutEvent* e = new TimeoutEvent(s, seconds, temp);
            dispatcher.enqueue(e);

            if (i == to_save) {
                saved = e;
            }
            events[i] = e;
        }

        CancelTimerEvent* cancel_event = new CancelTimerEvent(saved);
        dispatcher.enqueue(cancel_event);

        struct timespec t;
        long int n = nano * max + nano;
        Utils::get_timespec_future_time(seconds, n, &t);

        for (int i = 0; i < max; ++i) {
            if (i == to_save) {
                continue;
            }

            bool timedout = helper.get_sem()->timed_wait(&t);
            if (!timedout) {
                TimerFiredEvent* e = (TimerFiredEvent*) helper.get_queue()->dequeue();
                Event* actual = e->get_timeout_event();
                Event* expected = events[i];
                EXPECT_EQ(expected, actual);
            } else {
                break;
            }
        }
    }
}

#endif /* TIMEOUTEVENTMANAGERTEST_H_ */
