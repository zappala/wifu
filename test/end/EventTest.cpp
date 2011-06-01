/*
 * EventTest.cpp
 *
 *  Created on: Mar 31, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "events/Event.h"
#include "Socket.h"
#include "IModule.h"
#include "StandardPortManagerCreator.h"

using namespace std;

namespace {

    class EventTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            PortManagerFactory::instance().set_creator(new StandardPortManagerCreator());
        }
        virtual void TearDown() {

        }
    };

    class StubEvent : public Event {
    public:

        StubEvent() : Event() {
        }

        StubEvent(Socket* s) : Event(s) {
        }

        void execute(IModule* m) {
        }

    };

    TEST_F(EventTest, SocketConstructor) {
        Socket* socket = new Socket(0, 0, 0);
        StubEvent event(socket);

        ASSERT_TRUE(*socket == *event.get_socket());

    }

    TEST_F(EventTest, BlankConstructor) {
        StubEvent event;

        ASSERT_THROW(event.get_socket(), WiFuException);

        Socket* socket = new Socket(1, 1, 1);
        event.set_socket(socket);

        ASSERT_TRUE(*socket == *event.get_socket());
    }

}
