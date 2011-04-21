#include "states/TimeWait.h"

TimeWait::TimeWait() : timeout_event_(0) {

}

TimeWait::~TimeWait() {

}

void TimeWait::enter(Context* c) {
    ConnectionManagerContext* ccc = (ConnectionManagerContext*) c;
    timeout_event_ = new TimeoutEvent(ccc->get_socket(), TWO_MSL, 0);
    Dispatcher::instance().enqueue(timeout_event_);
}

void TimeWait::exit(Context* c) {

}

void TimeWait::timer_fired(Context* c, TimerFiredEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    Socket* s = e->get_socket();
    TimeoutEvent* event = e->get_timeout_event();

    if (event == timeout_event_) {
        Dispatcher::instance().enqueue(new DeleteSocketEvent(s));
        cmc->set_state(new Closed());
    }
}
