#include "contexts/TCPTahoeReliabilityContext.h"

TCPTahoeReliabilityContext::TCPTahoeReliabilityContext(u_int32_t iss) : OutstandingDataContext(iss), initialized_(false), rcv_nxt_(0), rcv_wnd_(MAX_TCP_RECEIVE_WINDOW_SIZE), timer_(0), duplicate_ack_number_(0), duplicates_(0), receive_event_(0), rto_(INITIAL_RTO), srtt_(-1.0), rttvar_(-1.0) {
    set_state(new TCPTahoeReliabilityState());
}

TCPTahoeReliabilityContext::~TCPTahoeReliabilityContext() {

}

bool TCPTahoeReliabilityContext::is_initialized() {
    return initialized_;
}

void TCPTahoeReliabilityContext::set_initialized() {
    initialized_ = true;
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

double TCPTahoeReliabilityContext::get_rto() {
    return rto_;
}

void TCPTahoeReliabilityContext::set_rto(double rto) {
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

TCPPacketBuffer& TCPTahoeReliabilityContext::get_receive_window() {
    return receive_window_;
}

ReceiveEvent* TCPTahoeReliabilityContext::get_receive_event() {
    return receive_event_;
}

void TCPTahoeReliabilityContext::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}

u_int32_t TCPTahoeReliabilityContext::get_echo_reply() {
    return echo_reply_;
}

void TCPTahoeReliabilityContext::set_echo_reply(u_int32_t echo_reply) {
    echo_reply_ = echo_reply;
}

double TCPTahoeReliabilityContext::get_srtt() {
    return srtt_;
}

void TCPTahoeReliabilityContext::set_srtt(double srtt) {
    srtt_ = srtt;
}

double TCPTahoeReliabilityContext::get_rttvar() {
    return rttvar_;
}

void TCPTahoeReliabilityContext::set_rttvar(double rttvar) {
    rttvar_ = rttvar;
}