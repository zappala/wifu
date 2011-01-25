/*
 * TimerFiredEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef TIMERFIREDEVENTTEST_H_
#define TIMERFIREDEVENTTEST_H_

#include "gtest/gtest.h"
#include "../headers/events/TimerFiredEvent.h"
#include "../headers/IModule.h"
#include "../headers/Socket.h"

using namespace std;

namespace {

    SUITE(TimerFiredEvent) {

        class IModuleDummyImplementation : public IModule {
        public:

            IModuleDummyImplementation() {
                timerFired = false;
            }

            void timer_fired(Event* e) {
                timerFired = true;
            }

            bool timerFired;
        };

        TEST(timer_fired) {
            IModuleDummyImplementation dummyImodule;
            CHECK(dummyImodule.timerFired == false);

            Socket* s = new Socket(1, 2, 3);
            TimeoutEvent timeoutEvent(s, 1, 0);
            TimerFiredEvent timerFiredEvent(&timeoutEvent);
            timerFiredEvent.execute(&dummyImodule);

            CHECK(dummyImodule.timerFired == true);
        }
    }
}

#endif /* TIMERFIREDEVENTTEST_H_ */
