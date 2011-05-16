#include "contexts/TCPTahoeReliabilityContext.h"

TCPTahoeReliabilityContext::TCPTahoeReliabilityContext() : snd_nxt_(0), snd_una_(0), rcv_nxt_(0), rcv_wnd_(MAX_TCP_RECEIVE_WINDOW_SIZE), timer_(0), duplicate_ack_number_(0), duplicates_(0) {
    set_state(new TCPTahoeReliabilityState());
}

TCPTahoeReliabilityContext::~TCPTahoeReliabilityContext() {

}

u_int32_t TCPTahoeReliabilityContext::get_snd_una() {
    return snd_una_;
}

void TCPTahoeReliabilityContext::set_snd_una(u_int32_t snd_una) {
    snd_una_ = snd_una;
}

u_int32_t TCPTahoeReliabilityContext::get_snd_nxt() {
    return snd_nxt_;
}

void TCPTahoeReliabilityContext::set_snd_nxt(u_int32_t snd_nxt) {
    snd_nxt_ = snd_nxt;
}

u_int32_t TCPTahoeReliabilityContext::get_rcv_nxt() {
    return rcv_nxt_;
}

void TCPTahoeReliabilityContext::set_rcv_nxt(u_int32_t rcv_nxt) {
    rcv_nxt_ = rcv_nxt;
}

u_int16_t TCPTahoeReliabilityContext::get_rcv_wnd() {
    return rcv_wnd_;
}

void TCPTahoeReliabilityContext::set_rcv_wnd(u_int16_t rcv_wnd) {
    rcv_wnd_ = rcv_wnd;
}

TimeoutEvent* TCPTahoeReliabilityContext::get_timeout_event() {
    return timer_;
}

void TCPTahoeReliabilityContext::set_timeout_event(TimeoutEvent* e) {
    timer_ = e;
}

int TCPTahoeReliabilityContext::get_rto() {
    return rto_;
}

void TCPTahoeReliabilityContext::set_rto(int rto) {
    rto_ = rto;
}

u_int32_t TCPTahoeReliabilityContext::get_duplicate_ack_number() {
    return duplicate_ack_number_;
}

void TCPTahoeReliabilityContext::set_duplicate_ack_number(u_int32_t num) {
    duplicate_ack_number_ = num;
}

int TCPTahoeReliabilityContext::get_duplicates() {
    return duplicates_;
}

void TCPTahoeReliabilityContext::set_duplicates(int duplicates) {
    duplicates_ = duplicates;
}

PriorityQueue<TCPPacket*, TCPSequenceNumberComparator>* TCPTahoeReliabilityContext::get_packet_buffer() {
    return &packet_buffer_;
}
