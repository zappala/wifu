#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context(), seq_counter_(0), last_packet_sent_(0), last_packet_received_(0) {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

}

void ReliabilityContext::socket(Socket* s) {
    get_state()->socket(this, s);
}

void ReliabilityContext::bind(Socket* s, AddressPort* ap) {
    get_state()->bind(this, s, ap);
}

void ReliabilityContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void ReliabilityContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void ReliabilityContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void ReliabilityContext::new_connection_established(Socket* s) {
    get_state()->new_connection_established(this, s);
}

void ReliabilityContext::close() {
    get_state()->close(this);
}

void ReliabilityContext::receive_packet(Socket* s, WiFuPacket* p) {
    get_state()->receive_packet(this, s, p);
}

void ReliabilityContext::send_packet(Socket* s, WiFuPacket* p) {
    get_state()->send_packet(this, s, p);
}

void ReliabilityContext::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void ReliabilityContext::resend_packet(Socket* s, WiFuPacket* p) {
    get_state()->resend_packet(this, s, p);
}

TCPPacket* ReliabilityContext::get_last_packet_sent() {
    return last_packet_sent_;
}

void ReliabilityContext::set_last_packet_sent(TCPPacket* p) {
    last_packet_sent_ = p;
}

TCPPacket* ReliabilityContext::get_last_packet_received() {
    return last_packet_received_;
}

void ReliabilityContext::set_last_packet_received(TCPPacket* p) {
    last_packet_received_ = p;
}

void ReliabilityContext::set_saved_timeout(TimeoutEvent* te) {
    saved_timeout_ = te;
}

TimeoutEvent* ReliabilityContext::get_saved_timeout() {
    return saved_timeout_;
}

void ReliabilityContext::set_seq_counter(u_int32_t value) {
    seq_counter_ = value;
}
u_int32_t ReliabilityContext::get_seq_counter() {
    return seq_counter_;
}