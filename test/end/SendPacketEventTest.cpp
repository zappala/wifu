/*
 * SendPacketEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef SENDPACKETEVENTTEST_H_
#define SENDPACKETEVENTTEST_H_

#include "gtest/gtest.h"
#include "events/SendPacketEvent.h"
#include "IModule.h"
#include "Socket.h"

using namespace std;

namespace {

    class IModuleDummyImplementation : public IModule {
    public:

        IModuleDummyImplementation() {
            sent = false;
        }

        void imodule_send(Event* e) {
            sent = true;
        }

        bool sent;
    };

    TEST(SendPacketEventTest, send) {
        IModuleDummyImplementation dummyImodule;
        ASSERT_TRUE(dummyImodule.sent == false);
        WiFuPacket* p;
        Socket* s = new Socket(1, 2, 3);
        SendPacketEvent sendPacketEvent(s, p);
        sendPacketEvent.execute(&dummyImodule);

        ASSERT_EQ(true, dummyImodule.sent);
    }
}

#endif /* SENDPACKETEVENTTEST_H_ */
