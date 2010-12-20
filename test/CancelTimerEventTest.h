/*
 * CancelTimerEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef CANCELTIMEREVENTTEST_H_
#define CANCELTIMEREVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/CancelTimerEvent.h"
#include "../headers/IModule.h"

using namespace std;

namespace {
	SUITE(CancelTimerEvent) {

		class IModuleDummyImplementation : public IModule {
		public:
			IModuleDummyImplementation() {
				timerCanceled = false;
			}

			void cancel_timer(Event* e) {
				timerCanceled = true;
			}

			bool timerCanceled;
		};

		TEST(cancel_timer) {
			IModuleDummyImplementation dummyImodule;
			CHECK(dummyImodule.timerCanceled == false);

			TimeoutEvent timeoutEvent(500, 1, 0);
			CancelTimerEvent cancelTimerEvent(&timeoutEvent);
			cancelTimerEvent.execute(&dummyImodule);

			CHECK(dummyImodule.timerCanceled == true);
		}
	}
}

#endif /* CANCELTIMEREVENTTEST_H_ */
