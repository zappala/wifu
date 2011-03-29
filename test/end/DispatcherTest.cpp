/*
 * DispatcherTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef DISPATCHERTEST_H_
#define DISPATCHERTEST_H_

#include "gtest/gtest.h"
#include "Dispatcher.h"

#include "events/NullEvent.h"
#include "events/ResponseEvent.h"

#include "Socket.h"
#include "defines.h"
#include "TimeoutEventManager.h"
#include "QueueProcessor.h"

#include <iostream>
#include <string>

using namespace std;

#define dispatcher Dispatcher::instance()
#define time_to_wait 5000

namespace {

	class TestProcessor : public QueueProcessor<Event*> {
	public:
		int events_processed;

		TestProcessor() : QueueProcessor<Event*>() {
			events_processed = 0;
		}

		void process(Event* event) {
			++events_processed;
		}
	};

	class StubEvent1 : public Event {
	public:
		StubEvent1() : Event() {}

		void execute(IModule* m) {}

	};

	class StubEvent2 : public Event {
	public:
		StubEvent2() : Event() {}

		void execute(IModule* m) {}

	};

	class StubEvent3 : public Event {
	public:
		StubEvent3() : Event() {}

		void execute(IModule* m) {}

	};

	TEST(Dispatcher, mapEvent) {
		StubEvent1 event1;

		TestProcessor processor1, processor2, processor3;
		processor1.start_processing();
		processor2.start_processing();
		processor3.start_processing();

		dispatcher.reset();
		dispatcher.start_processing();

		ASSERT_EQ(0, processor1.events_processed);

		dispatcher.map_event(type_name(StubEvent1), &processor1);
		dispatcher.enqueue(&event1);
		usleep(time_to_wait);

		ASSERT_EQ(1, processor1.events_processed);

		dispatcher.map_event(type_name(StubEvent1), &processor1);
		dispatcher.enqueue(&event1);
		usleep(time_to_wait);

		ASSERT_EQ(2, processor1.events_processed);

		dispatcher.map_event(type_name(StubEvent1), &processor2);
		dispatcher.map_event(type_name(StubEvent1), &processor3);
		ASSERT_EQ(0, processor2.events_processed);
		ASSERT_EQ(0, processor3.events_processed);

		dispatcher.enqueue(&event1);
		usleep(time_to_wait);

		ASSERT_EQ(3, processor1.events_processed);
		ASSERT_EQ(1, processor2.events_processed);
		ASSERT_EQ(1, processor3.events_processed);

		dispatcher.map_event(type_name(StubEvent1), &processor2);
		dispatcher.map_event(type_name(StubEvent1), &processor3);

		dispatcher.enqueue(&event1);
		usleep(time_to_wait);

		ASSERT_EQ(4, processor1.events_processed);
		ASSERT_EQ(2, processor2.events_processed);
		ASSERT_EQ(2, processor3.events_processed);

		dispatcher.map_event(type_name(StubEvent2), &processor2);
		StubEvent2 event2;

		dispatcher.enqueue(&event2);
		usleep(time_to_wait);

		ASSERT_EQ(4, processor1.events_processed);
		ASSERT_EQ(3, processor2.events_processed);
		ASSERT_EQ(2, processor3.events_processed);

		StubEvent3 event3;

		dispatcher.enqueue(&event3);
		usleep(time_to_wait);
	}

}

#endif /* DISPATCHERTEST_H_ */
