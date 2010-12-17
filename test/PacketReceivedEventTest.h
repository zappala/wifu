/*
 * PacketReceivedEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef PACKETRECEIVEDEVENTTEST_H_
#define PACKETRECEIVEDEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/PacketReceivedEvent.h"
#include "../headers/IModule.h"

using namespace std;

namespace {
	SUITE(PacketReceivedEvent) {

		class IModuleDummyImplementation : public IModule {
		public:
			IModuleDummyImplementation() {
				received = false;
			}

			void receive(Event* e) {
				received = true;
			}

			bool received;
		};

		TEST(receive) {
			IModuleDummyImplementation dummyImodule;
			CHECK(dummyImodule.received == false);

			PacketReceivedEvent packetReceivedEvent(100);
			packetReceivedEvent.execute(&dummyImodule);

			CHECK(dummyImodule.received == true);
		}
	}
}

#endif /* PACKETRECEIVEDEVENTTEST_H_ */
