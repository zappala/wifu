/*
 * CancelTimerEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef CANCELTIMEREVENTTEST_H_
#define CANCELTIMEREVENTTEST_H_

#include "gtest/gtest.h"
#include "../headers/events/TimeoutEvent.h"
#include "../headers/events/CancelTimerEvent.h"
#include "../headers/IModule.h"
#include "../headers/Socket.h"

using namespace std;

namespace {
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

	TEST(CancelTimerEventTest, cancel_timer) {
		IModuleDummyImplementation dummyImodule;
		ASSERT_TRUE(dummyImodule.timerCanceled == false);

		Socket* s = new Socket(0, 1, 2);
		TimeoutEvent timeoutEvent(s, 1, 0);
		CancelTimerEvent cancelTimerEvent(&timeoutEvent);
		cancelTimerEvent.execute(&dummyImodule);

		ASSERT_TRUE(dummyImodule.timerCanceled == true);
	}
}

#endif /* CANCELTIMEREVENTTEST_H_ */
