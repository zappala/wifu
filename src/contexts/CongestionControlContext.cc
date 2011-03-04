#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context() {
    set_state(new SlowStart());
}

void CongestionControlContext::socket(SocketEvent* e) {
    get_state()->socket(this, e);
}

void CongestionControlContext::bind(BindEvent* e) {
    get_state()->bind(this, e);
}

void CongestionControlContext::listen(ListenEvent* e) {
    get_state()->listen(this, e);
}

void CongestionControlContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void CongestionControlContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void CongestionControlContext::new_connection_established(ConnectionEstablishedEvent* e) {
    get_state()->new_connection_established(this, e);
}

void CongestionControlContext::close() {
    get_state()->close(this);
}

void CongestionControlContext::receive_packet(NetworkReceivePacketEvent* e) {
    get_state()->receive_packet(this, e);
}

void CongestionControlContext::send_packet(SendPacketEvent* e) {
    get_state()->send_packet(this, e);
}

void CongestionControlContext::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void CongestionControlContext::resend_packet(ResendPacketEvent* e) {
    get_state()->resend_packet(this, e);
}
