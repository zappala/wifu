/*
 * TimeoutEventTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef TIMEOUTEVENTTEST_H_
#define TIMEOUTEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/TimeoutEvent.h"
#include "../headers/IModule.h"
#include <time.h>

using namespace std;

namespace {
	SUITE(TimeoutEvent) {

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

		TEST(timeout) {
			IModuleDummyImplementation dummyImodule;
			CHECK(dummyImodule.timedout == false);

			TimeoutEvent timeoutEvent(500, 1, 0);
			timeoutEvent.execute(&dummyImodule);

			CHECK(dummyImodule.timedout == true);
		}

		TEST(get_timeout_time) {
			timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);

			TimeoutEvent timeoutEvent(500, 2, 0);

			CHECK(timeoutEvent.get_timeout_time().tv_sec > ts.tv_sec);
		}
	}
}

#endif /* TIMEOUTEVENTTEST_H_ */
