#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context(), seq_counter_(1), last_packet_sent_(0), last_packet_received_(0) {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

}

void ReliabilityContext::socket(SocketEvent* e) {
    get_state()->socket(this, e);
}

void ReliabilityContext::bind(BindEvent* e) {
    get_state()->bind(this, e);
}

void ReliabilityContext::listen(ListenEvent* e) {
    get_state()->listen(this, e);
}

void ReliabilityContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void ReliabilityContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void ReliabilityContext::new_connection_established(ConnectionEstablishedEvent* e) {
    get_state()->new_connection_established(this, e);
}

void ReliabilityContext::close() {
    get_state()->close(this);
}

void ReliabilityContext::receive_packet(NetworkReceivePacketEvent* e) {
    get_state()->receive_packet(this, e);
}

void ReliabilityContext::send_packet(SendPacketEvent* e) {
    get_state()->send_packet(this, e);
}

void ReliabilityContext::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void ReliabilityContext::resend_packet(ResendPacketEvent* e) {
    get_state()->resend_packet(this, e);
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