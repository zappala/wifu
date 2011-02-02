/*
 * PacketReceivedEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef PACKETRECEIVEDEVENTTEST_H_
#define PACKETRECEIVEDEVENTTEST_H_

#include "gtest/gtest.h"
#include "events/PacketReceivedEvent.h"
#include "Socket.h"
#include "IModule.h"

using namespace std;

namespace {

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

    TEST(PacketReceivedEventTest, receive) {
        IModuleDummyImplementation dummyImodule;
        ASSERT_TRUE(dummyImodule.received == false);

        Socket* s = new Socket(1, 2, 3);
        PacketReceivedEvent packetReceivedEvent(s);
        packetReceivedEvent.execute(&dummyImodule);

        ASSERT_TRUE(dummyImodule.received == true);
    }
}

#endif /* PACKETRECEIVEDEVENTTEST_H_ */
