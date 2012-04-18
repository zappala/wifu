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
    char buffer[100];
    sprintf(buffer, "Seconds: %ld Nanoseconds: %ld", timer_.tv_sec, timer_.tv_nsec);
    return buffer;
}
