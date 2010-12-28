/*
 * TimerFiredEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef TIMERFIREDEVENTTEST_H_
#define TIMERFIREDEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/events/TimerFiredEvent.h"
#include "../headers/IModule.h"

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

			TimeoutEvent timeoutEvent(500, 1, 0);
			TimerFiredEvent timerFiredEvent(&timeoutEvent);
			timerFiredEvent.execute(&dummyImodule);

			CHECK(dummyImodule.timerFired == true);
		}
	}
}

#endif /* TIMERFIREDEVENTTEST_H_ */
