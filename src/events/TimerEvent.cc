#include "events/TimerEvent.h"

TimerEvent::TimerEvent(TimeoutEvent* timeout_event) : Event(timeout_event->get_socket()), timeout_event_(timeout_event) {}

TimerEvent::~TimerEvent() {}

TimeoutEvent* TimerEvent::get_timeout_event() {
	return timeout_event_;
}
