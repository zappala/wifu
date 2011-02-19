#include "TimerFiredEvent.h"

TimerFiredEvent::TimerFiredEvent(TimeoutEvent* timeout_event) : TimerEvent(timeout_event) {}

TimerFiredEvent::~TimerFiredEvent() {}

void TimerFiredEvent::execute(IModule* m) {
	m->timer_fired(this);
}
