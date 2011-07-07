#include "events/framework_events/TimeoutEvent.h"

TimeoutEvent::TimeoutEvent(Socket* socket, int seconds, long int nanoseconds) : FrameworkEvent(socket) {
    Utils::get_timespec_future_time(seconds, nanoseconds, &timer_);
}

struct timespec& TimeoutEvent::get_timeout_time() {
    return timer_;
}

void TimeoutEvent::execute(IModule* m) {
    m->imodule_timeout(this);
}

gcstring TimeoutEvent::to_s() {
    stringstream s;
    s << "Seconds: " << timer_.tv_sec << " Nanoseconds: " << timer_.tv_nsec;
    return s.str();
}
