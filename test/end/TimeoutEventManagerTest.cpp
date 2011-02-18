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

#define dispatcher Dispatcher::instance()
#define timer TimeoutEventManager::instance()


using namespace std;

namespace {

    class TimeoutEventManagerHelper : public Module {
    private:

        Semaphore* sem;

        TimeoutEventManagerHelper() {
            reset();
            q_ = new Queue<Event*>();
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
        ASSERT_FALSE(timedout);

        TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
        TimeoutEvent* actual = event->get_timeout_event();
        ASSERT_EQ(expected, actual);

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
        ASSERT_FALSE(timedout);

        TimerFiredEvent* event = (TimerFiredEvent*) helper.get_queue()->dequeue();
        TimeoutEvent* actual = event->get_timeout_event();
        ASSERT_EQ(expected, actual);
    }

    TEST_F(TimeoutEventManagerTest, EnqueueTwo) {
        
    }

    TEST_F(TimeoutEventManagerTest, MassEnqueue) {

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
        ASSERT_TRUE(timedout);

        ASSERT_TRUE(helper.get_queue()->size() == 0);
    }

    TEST_F(TimeoutEventManagerTest, CancelTimer2) {
        int max = 500;
        int to_save = max - (max / 2);
        assert(to_save < max);
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;

        Event* events[max];

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
                ASSERT_EQ(expected, actual);
            } else {
                break;
            }
        }
    }
}

#endif /* TIMEOUTEVENTMANAGERTEST_H_ */
