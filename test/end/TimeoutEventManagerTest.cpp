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
#include <map>
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
            srand(time(NULL));
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

    TEST_F(TimeoutEventManagerTest, CancelOneTimerOutOfMany) {
        int max = 10;
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

    TEST_F(TimeoutEventManagerTest, CancelManyTimersOutOfMany) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 1;
        long int nano = 1000;
        int max = 10000;
        vector<Event*> events;
        HashSet<int> to_cancel;

        for (int i = 0; i < max; ++i) {
            long int time = (nano * i) + nano;
            TimeoutEvent* e = new TimeoutEvent(s, seconds, time);
            events.push_back(e);
        }

        vector<Event*> events_copy(events);

        // Randomly insert
        while(!events.empty()) {
            int random = get_random(events.size());
            Event* e = events.at(random);
            events.erase(events.begin() + random);
            dispatcher.enqueue(e);
        }

        // Randomly cancel up to half the timers
        while(to_cancel.size() < events_copy.size() / 2) {
            int random = get_random(events_copy.size());
            if(to_cancel.contains(random)) {
                continue;
            }
            
            TimeoutEvent* event = (TimeoutEvent*) events_copy.at(random);
            to_cancel.insert(random);
            Event* e = new CancelTimerEvent(event);
            dispatcher.enqueue(e);
        }


        int fired_events = 0;

        for (int i = 0; i < max; ++i) {
            if(to_cancel.contains(i)) {
                continue;
            }

            helper.get_sem()->wait();
            TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
            ++fired_events;
            Event* expected = events_copy.at(i);
            Event* actual = event->get_timeout_event();
            EXPECT_EQ(expected, actual);
        }

//        cout << "Fired events: " << fired_events << endl;
//        cout << "Canceled size: " << to_cancel.size() << endl;
        ASSERT_EQ(max, to_cancel.size() + fired_events);
    }

    // Fires a timer in 1 millisecond
    // Tests to ensure that it was received in precision time (measured in nanoseconds)
    // Can be used to see how granual timers can be made
    // Note also that the results of this are the best possible as all queues are empty
    // and therefore no queuing delay will be introduced
    void precision_test(int precision) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        // 1 millisecond
        int nano = 1000000;
        TimeoutEvent* expected = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(expected);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano + precision, &t);

        // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
        bool timedout = helper.get_sem()->timed_wait(&t);
        EXPECT_FALSE(timedout);

        TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
        TimeoutEvent* actual = event->get_timeout_event();
        EXPECT_EQ(expected, actual);
    }

    TEST_F(TimeoutEventManagerTest, PrecisionTest10Milliseconds) {
        // 10 milliseconds in nanoseconds
        precision_test(10000000);
    }

    TEST_F(TimeoutEventManagerTest, PrecisionTest5Milliseconds) {
        // 5 milliseconds in nanoseconds
        precision_test(5000000);
    }

    TEST_F(TimeoutEventManagerTest, PrecisionTest1Millisecond) {
        // 1 millisecond in nanoseconds
        precision_test(1000000);
    }

    TEST_F(TimeoutEventManagerTest, PrecisionTest500Microseconds) {
        // 500 microseconds in nanoseconds
        precision_test(500000);
    }

    TEST_F(TimeoutEventManagerTest, PrecisionTest100Microseconds) {
        // 100 microseconds in nanoseconds
        precision_test(100000);
    }

    // These tests fail sometimes
//    TEST_F(TimeoutEventManagerTest, PrecisionTest50Microseconds) {
//        // 50 microseconds in nanoseconds
//        precision_test(50000);
//    }
//
//    TEST_F(TimeoutEventManagerTest, PrecisionTest10Microseconds) {
//        // 10 microseconds in nanoseconds
//        precision_test(10000);
//    }
//
//    TEST_F(TimeoutEventManagerTest, PrecisionTest1Microsecond) {
//        // 1 microseconds in nanoseconds
//        precision_test(1000);
//    }
}

#endif /* TIMEOUTEVENTMANAGERTEST_H_ */
