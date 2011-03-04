#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context(), seq_counter_(1), last_packet_sent_(0), last_packet_received_(0) {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

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