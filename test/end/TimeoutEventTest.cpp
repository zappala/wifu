/*
 * TimeoutEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef TIMEOUTEVENTTEST_H_
#define TIMEOUTEVENTTEST_H_

#include "gtest/gtest.h"
#include "../headers/events/TimeoutEvent.h"
#include "../headers/IModule.h"
#include "../headers/Socket.h"
#include <time.h>

using namespace std;

namespace {
	class IModuleDummyImplementation : public IModule {
	public:
		IModuleDummyImplementation() {
			timedout = false;
		}

		void timeout(Event* e) {
			timedout = true;
		}

		bool timedout;
	};

	TEST(TimeoutEventTest, timeout) {
		IModuleDummyImplementation dummyImodule;
		ASSERT_TRUE(dummyImodule.timedout == false);

					Socket* s = new Socket(0,1,2);
		TimeoutEvent timeoutEvent(s, 1, 0);
		timeoutEvent.execute(&dummyImodule);

		ASSERT_TRUE(dummyImodule.timedout == true);
	}

	TEST(TimeoutEventTest, get_timeout_time) {
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);

					Socket* s = new Socket(0,1,2);
		TimeoutEvent timeoutEvent(s, 2, 0);

		ASSERT_TRUE(timeoutEvent.get_timeout_time().tv_sec > ts.tv_sec);
	}
}

#endif /* TIMEOUTEVENTTEST_H_ */
