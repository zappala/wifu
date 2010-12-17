/*
 * ConnectEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef CONNECTEVENTTEST_H_
#define CONNECTEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/ConnectEvent.h"
#include "../headers/IModule.h"

using namespace std;

namespace {
	SUITE(ConnectEvent) {

		class IModuleDummyImplementation : public IModule {
		public:
			IModuleDummyImplementation() {
				received = false;
			}

			void connect(Event* e) {
				received = true;
			}

			bool received;
		};

		TEST(connect) {
			IModuleDummyImplementation dummyImodule;
			CHECK(dummyImodule.received == false);

			string address("127.0.0.1");
			int port = 100;
			ConnectEvent connectEvent(100, address, port);
			connectEvent.execute(&dummyImodule);

			CHECK(dummyImodule.received == true);
		}
	}
}

#endif /* CONNECTEVENTTEST_H_ */
