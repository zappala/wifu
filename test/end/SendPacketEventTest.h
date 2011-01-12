/*
 * SendPacketEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef SENDPACKETEVENTTEST_H_
#define SENDPACKETEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/events/SendPacketEvent.h"
#include "../headers/IModule.h"

using namespace std;

namespace {

    SUITE(SendPacketEvent) {

        class IModuleDummyImplementation : public IModule {
        public:

            IModuleDummyImplementation() {
                sent = false;
            }

            void send(Event* e) {
                sent = true;
            }

            bool sent;
        };

        TEST(send) {
            IModuleDummyImplementation dummyImodule;
            CHECK(dummyImodule.sent == false);
            Packet* p;
            SendPacketEvent sendPacketEvent(100, p);
            sendPacketEvent.execute(&dummyImodule);

            CHECK_EQUAL(true, dummyImodule.sent);
        }
    }
}

#endif /* SENDPACKETEVENTTEST_H_ */
