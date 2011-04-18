#include "states/TimeWait.h"

TimeWait::TimeWait() {

}

TimeWait::~TimeWait() {

}

void TimeWait::enter(Context* c) {

}

void TimeWait::exit(Context* c) {

}

void TimeWait::timer_fired(Context* c, TimerFiredEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    Socket* s = e->get_socket();
    TimeoutEvent* event = e->get_timeout_event();
}
