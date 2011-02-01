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
#include "../headers/Socket.h"

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
            WiFuPacket* p;
            Socket* s = new Socket(1, 2, 3);
            SendPacketEvent sendPacketEvent(s, p);
            sendPacketEvent.execute(&dummyImodule);

            CHECK_EQUAL(true, dummyImodule.sent);
        }
    }
}

#endif /* SENDPACKETEVENTTEST_H_ */
