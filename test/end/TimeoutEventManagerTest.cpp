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
        TimerFiredEvent* event_;

        TimeoutEventManagerHelper() {
            reset();
        }

    public:

        static TimeoutEventManagerHelper& instance() {
            static TimeoutEventManagerHelper instance_;
            return instance_;
        }

        void timer_fired(Event* e) {
            cout << "Timer Fired" << endl;
            TimerFiredEvent* event = (TimerFiredEvent*) e;
            event_ = event;
            sem->post();
        }

        Semaphore* get_sem() {
            return sem;
        }

        void reset() {
            sem = new Semaphore();
            sem->init(0);
        }

        TimerFiredEvent* get_last_timer_fired_event() {
            return event_;
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

    TEST_F(TimeoutEventManagerTest, EnqueueDirectly) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        int nano = 10000000;
        TimeoutEvent* e = new TimeoutEvent(s, seconds, nano);
        timer.enqueue(e);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
        bool timedout = helper.get_sem()->timed_wait(&t);
        ASSERT_FALSE(timedout);
    }

    TEST_F(TimeoutEventManagerTest, DispatcherEnqueue) {
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;
        int nano = 10000000;
        TimeoutEvent* e = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(e);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        // This timeout indicates the maximum time we will wait for the TimerFiredEvent to occur
        bool timedout = helper.get_sem()->timed_wait(&t);
        ASSERT_FALSE(timedout);
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
        Utils::get_timespec_future_time(seconds, nano * 50, &t);

        bool timedout = helper.get_sem()->timed_wait(&t);
        ASSERT_TRUE(timedout);
    }

    TEST_F(TimeoutEventManagerTest, CancelTimer2) {
        int max = 5;
        int to_save = max - 2;
        assert(to_save < max);
        Socket* s = new Socket(0, 0, 0);
        int seconds = 0;

        Event * events[max];

        // 10 ms
        int nano = 10000000;

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
            Event* temp_event = events[i];
            cout << "Array:  " << temp_event << endl;
            cout << "Pointer:  " << e << endl;
        }

        CancelTimerEvent* cancel_event = new CancelTimerEvent(saved);
        dispatcher.enqueue(cancel_event);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 10, &t);

        for (int i = 0; i < max; ++i) {
            if (i == to_save) {
                continue;
            }

            bool timedout = helper.get_sem()->timed_wait(&t);
            if (!timedout) {
                TimerFiredEvent* e = helper.get_last_timer_fired_event();
                Event* original = e->get_timeout_event();
                Event* temp_event = events[i];
                cout << "Array:  " << temp_event << endl;
                cout << "Pointer:  " << original << endl;
                ASSERT_EQ(temp_event, original);
            } else {
                break;
            }
        }
    }
}

#endif /* TIMEOUTEVENTMANAGERTEST_H_ */
