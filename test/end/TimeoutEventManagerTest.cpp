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

        Semaphore sem;

        TimeoutEventManagerHelper() {
            sem.init(0);
        }

    public:

        static TimeoutEventManagerHelper& instance() {
            static TimeoutEventManagerHelper instance_;
            return instance_;
        }

        void timer_fired(Event* e) {
            TimerFiredEvent* event = (TimerFiredEvent*) e;
            cout << "Timer Fired" << endl;
            sem.post();
        }

        Semaphore& get_sem() {
            return sem;
        }


    };

    class TimeoutEventManagerTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            dispatcher.map_event(type_name(TimeoutEvent), &timer);
            dispatcher.map_event(type_name(CancelTimerEvent), &timer);
            dispatcher.map_event(type_name(TimerFiredEvent), &TimeoutEventManagerHelper::instance());
            dispatcher.start_processing();
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TimeoutEventManagerTest, EnqueueDirectly) {
        Socket* s = new Socket(0,0,0);
        int seconds = 0;
        int nano = 1000000;
        TimeoutEvent* e = new TimeoutEvent(s, seconds, nano);
        timer.enqueue(e);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        bool timedout = TimeoutEventManagerHelper::instance().get_sem().timed_wait(&t);
        ASSERT_FALSE(timedout);
    }

    TEST_F(TimeoutEventManagerTest, DispatcherEnqueue) {
        Socket* s = new Socket(0,0,0);
        int seconds = 0;
        int nano = 1000000;
        TimeoutEvent* e = new TimeoutEvent(s, seconds, nano);
        dispatcher.enqueue(e);

        struct timespec t;
        Utils::get_timespec_future_time(seconds, nano * 2, &t);

        bool timedout = TimeoutEventManagerHelper::instance().get_sem().timed_wait(&t);
        ASSERT_FALSE(timedout);
    }
}

#endif /* TIMEOUTEVENTMANAGERTEST_H_ */
