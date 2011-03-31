/*
 * ModuleTest.cpp
 *
 *  Created on: Mar 31, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "Module.h"
#include "Dispatcher.h"
#include "events/NullEvent.h"

using namespace std;

namespace {

	class StubEvent : public Event {
	public:
		StubEvent() : Event() {}
		void execute(IModule* m) {}
	};

	class StubQueueProcessor : public QueueProcessor<Event*> {
	public:
		int received;

		StubQueueProcessor() : QueueProcessor<Event*>() {
			received = 0;
		}

		void enqueue(Event* object, bool signal = false) {
			++received;
		}

		void process(Event* object) {}

	};

	TEST(Module, dispatch) {
//		Module module;
//		Dispatcher::instance().reset();
//		StubQueueProcessor qp;
//		StubEvent* event = new StubEvent();
//
//		Dispatcher::instance().map_event(type_name(StubEvent), &qp);
//		Dispatcher::instance().start_processing();
//
//		ASSERT_EQ(0, qp.received);
//
//		module.dispatch(event);
//		usleep(1000);
//
//		ASSERT_EQ(1, qp.received);
//
//		Dispatcher::instance().reset();
	}

}
