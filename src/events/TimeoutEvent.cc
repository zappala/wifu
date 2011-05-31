#include "events/TimeoutEvent.h"

TimeoutEvent::TimeoutEvent(Socket* socket, int seconds, long int nanoseconds) : Event(socket) {
	Utils::get_timespec_future_time(seconds, nanoseconds, &timer_);
}

struct timespec& TimeoutEvent::get_timeout_time() {
	return timer_;
}

bool TimeoutEvent::less_than(Event* e) {
	TimeoutEvent* rhs = (TimeoutEvent*) e;

	struct timespec * a = &(get_timeout_time());
	struct timespec * b = &(rhs->get_timeout_time());

	if (a->tv_sec < b->tv_sec) return true;
	if (a->tv_sec == b->tv_sec && a->tv_nsec < b->tv_nsec) return true;
	return false;
}

void TimeoutEvent::execute(IModule* m) {
	m->imodule_timeout(this);
}

string TimeoutEvent::to_s() {
    stringstream s;
    s << "Seconds: " << timer_.tv_sec << " Nanoseconds: " << timer_.tv_nsec;
    return s.str();
}
