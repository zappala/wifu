#include "events/framework_events/TimerFiredEvent.h"

TimerFiredEvent::TimerFiredEvent(TimeoutEvent* timeout_event) : TimerEvent(timeout_event) {}

TimerFiredEvent::~TimerFiredEvent() {}

void TimerFiredEvent::execute(IModule* m) {
	m->imodule_timer_fired(this);
}
