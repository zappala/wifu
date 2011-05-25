#include "states/TimeWait.h"

TimeWait::TimeWait() : timeout_event_(0) {

}

TimeWait::~TimeWait() {

}

void TimeWait::state_enter(Context* c) {
    start_timer(c);
}

void TimeWait::state_exit(Context* c) {
    stop_timer(c);
}

void TimeWait::state_timer_fired(Context* c, TimerFiredEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    Socket* s = e->get_socket();
    TimeoutEvent* event = e->get_timeout_event();

    if (event == timeout_event_) {
        Dispatcher::instance().enqueue(new DeleteSocketEvent(s));
        cmc->set_state(new Closed());
    }
}

void TimeWait::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    //    RFC p. 73:
    //    The only thing that can arrive in this state is a
    //    retransmission of the remote FIN. Acknowledge it, and restart
    //    the 2 MSL timeout.
    cout << "TimeWait::state_receive_packet()" << endl;

    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_fin()) {

        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        TCPPacket* response = new TCPPacket();
        response->insert_tcp_header_option(new TCPTimestampOption());
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());

        response->set_data(data, 0);

        SendPacketEvent* event = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(event);

        restart_timer(c);
    }
}

void TimeWait::start_timer(Context* c) {
    ConnectionManagerContext* ccc = (ConnectionManagerContext*) c;
    timeout_event_ = new TimeoutEvent(ccc->get_socket(), TWO_MSL, 0);
    Dispatcher::instance().enqueue(timeout_event_);
}

void TimeWait::stop_timer(Context* c) {
    Dispatcher::instance().enqueue(new CancelTimerEvent(timeout_event_));
    timeout_event_ = 0;
}

void TimeWait::restart_timer(Context* c) {
    stop_timer(c);
    start_timer(c);
}
