/* 
 * File:   TCPDelayedACKReliabilityContext.cc
 * Author: leer
 * 
 * Created on June 17, 2011, 2:09 PM
 */

#include "contexts/TCPDelayedACKReliabilityContext.h"

TCPDelayedACKReliabilityContext::TCPDelayedACKReliabilityContext(u_int32_t iss) : OutstandingDataContext(iss), initialized_(false), rcv_nxt_(0), rcv_wnd_(MAX_TCP_RECEIVE_WINDOW_SIZE), timer_(0), duplicate_ack_number_(0), duplicates_(0), receive_event_(0), rto_(INITIAL_RTO), delay_timeout_interval_(INITAL_DELAY_TIMEOUT_INTERVAL), srtt_(-1.0), rttvar_(-1.0), delay_count_(DEFAULT_DELAY_COUNT) {
    set_state(new TCPDelayedACKReliabilityState());
}

TCPDelayedACKReliabilityContext::~TCPDelayedACKReliabilityContext() {

}

bool TCPDelayedACKReliabilityContext::is_initialized() {
    return initialized_;
}

void TCPDelayedACKReliabilityContext::set_initialized() {
    initialized_ = true;
}

u_int32_t TCPDelayedACKReliabilityContext::get_rcv_nxt() {
    return rcv_nxt_;
}

void TCPDelayedACKReliabilityContext::set_rcv_nxt(u_int32_t rcv_nxt) {
    rcv_nxt_ = rcv_nxt;
}

u_int16_t TCPDelayedACKReliabilityContext::get_rcv_wnd() {
    return rcv_wnd_;
}

void TCPDelayedACKReliabilityContext::set_rcv_wnd(u_int16_t rcv_wnd) {
    rcv_wnd_ = rcv_wnd;
}

TimeoutEvent* TCPDelayedACKReliabilityContext::get_timeout_event() {
    return timer_;
}

void TCPDelayedACKReliabilityContext::set_timeout_event(TimeoutEvent* e) {
    timer_ = e;
}

TimeoutEvent* TCPDelayedACKReliabilityContext::get_ack_timeout_event() {
    return ack_timer_;
}

void TCPDelayedACKReliabilityContext::set_ack_timeout_event(TimeoutEvent* e) {
    ack_timer_ = e;
}

double TCPDelayedACKReliabilityContext::get_rto() {
    return rto_;
}

void TCPDelayedACKReliabilityContext::set_rto(double rto) {
    rto_ = rto;
}

u_int32_t TCPDelayedACKReliabilityContext::get_duplicate_ack_number() {
    return duplicate_ack_number_;
}

void TCPDelayedACKReliabilityContext::set_duplicate_ack_number(u_int32_t num) {
    duplicate_ack_number_ = num;
}

int TCPDelayedACKReliabilityContext::get_duplicates() {
    return duplicates_;
}

void TCPDelayedACKReliabilityContext::set_duplicates(int duplicates) {
    duplicates_ = duplicates;
}

TCPPacketBuffer& TCPDelayedACKReliabilityContext::get_receive_window() {
    return receive_window_;
}

ReceiveEvent* TCPDelayedACKReliabilityContext::get_receive_event() {
    return receive_event_;
}

void TCPDelayedACKReliabilityContext::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}

u_int32_t TCPDelayedACKReliabilityContext::get_echo_reply() {
    return echo_reply_;
}

void TCPDelayedACKReliabilityContext::set_echo_reply(u_int32_t echo_reply) {
    echo_reply_ = echo_reply;
}

double TCPDelayedACKReliabilityContext::get_srtt() {
    return srtt_;
}

void TCPDelayedACKReliabilityContext::set_srtt(double srtt) {
    srtt_ = srtt;
}

double TCPDelayedACKReliabilityContext::get_rttvar() {
    return rttvar_;
}

void TCPDelayedACKReliabilityContext::set_rttvar(double rttvar) {
    rttvar_ = rttvar;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay_count(){
    return delay_count_;
}

void TCPDelayedACKReliabilityContext::set_delay_count(u_int16_t count) {
    delay_count_ = count;
}

double TCPDelayedACKReliabilityContext::get_delay_timeout_interval(){
    return delay_timeout_interval_;
}

void TCPDelayedACKReliabilityContext::set_delay_timeout_interval(double seconds) {
    delay_timeout_interval_ = seconds;
}
