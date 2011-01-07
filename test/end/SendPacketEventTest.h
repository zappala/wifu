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
                udp_sent = false;
            }

            void udp_send(Event* e) {
                udp_sent = true;
            }

            bool udp_sent;
        };

        TEST(udp_send) {
            IModuleDummyImplementation dummyImodule;
            CHECK(dummyImodule.udp_sent == false);
            Packet* p;
            SendPacketEvent sendPacketEvent(100, p);
            sendPacketEvent.execute(&dummyImodule);

            CHECK_EQUAL(true, dummyImodule.udp_sent);
        }
    }
}

#endif /* SENDPACKETEVENTTEST_H_ */
