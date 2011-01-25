/*
 * SendPacketEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef SENDPACKETEVENTTEST_H_
#define SENDPACKETEVENTTEST_H_

#include "gtest/gtest.h"
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
            ASSERT_TRUE(dummyImodule.sent == false);
            Packet* p;
            Socket* s = new Socket(1, 2, 3);
            SendPacketEvent sendPacketEvent(s, p);
            sendPacketEvent.execute(&dummyImodule);

            ASSERT_EQ(true, dummyImodule.sent);
        }
    }
}

#endif /* SENDPACKETEVENTTEST_H_ */
