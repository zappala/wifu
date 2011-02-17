#include "TimeoutHelper.h"

TimeoutHelper::TimeoutHelper() : HashSet<Event*>() {}

TimeoutHelper::~TimeoutHelper() {}

void TimeoutHelper::dispatch_timeout(Event* event) {
	insert(event);
	Dispatcher::instance().enqueue(event);
}

bool TimeoutHelper::is_my_timeout(TimerFiredEvent* event) {
	bool value = contains(event->get_timeout_event());
	remove(event);
	return value;
}

void TimeoutHelper::cancel_timeout(TimeoutEvent* event) {
	remove(event);
	Dispatcher::instance().enqueue(new CancelTimerEvent(event));
}
