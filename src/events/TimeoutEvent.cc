#include "events/TimeoutEvent.h"

TimeoutEvent::TimeoutEvent(Socket* socket, int seconds, long int nanoseconds) : Event(socket) {
	Utils::get_timespec_future_time(seconds, nanoseconds, &timer_);
}

struct timespec& TimeoutEvent::get_timeout_time() {
	return timer_;
}

bool TimeoutEvent::less_than(Event* e) {
    assert(false);
}

void TimeoutEvent::execute(IModule* m) {
	m->imodule_timeout(this);
}

string TimeoutEvent::to_s() {
    stringstream s;
    s << "Seconds: " << timer_.tv_sec << " Nanoseconds: " << timer_.tv_nsec;
    return s.str();
}
