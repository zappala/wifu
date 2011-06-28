/* 
 * File:   SimpleUDPReliabilityContext.cc
 * Author: leer
 * 
 * Created on June 21, 2011, 10:17 AM
 */

#include "contexts/SimpleUDPReliabilityContext.h"

SimpleUDPReliabilityContext::SimpleUDPReliabilityContext() : Context(), initialized_(false), rcv_nxt_(0), receive_event_(0) {
    set_state(new SimpleUDPReliabilityState());
}

SimpleUDPReliabilityContext::~SimpleUDPReliabilityContext() {

}

bool SimpleUDPReliabilityContext::is_initialized() {
    return initialized_;
}

void SimpleUDPReliabilityContext::set_initialized() {
    initialized_ = true;
}

//u_int32_t SimpleUDPReliabilityContext::get_rcv_nxt() {
//    return rcv_nxt_;
//}
//
//void SimpleUDPReliabilityContext::set_rcv_nxt(u_int32_t rcv_nxt) {
//    rcv_nxt_ = rcv_nxt;
//}
//
//u_int16_t SimpleUDPReliabilityContext::get_rcv_wnd() {
//    return rcv_wnd_;
//}
//
//void SimpleUDPReliabilityContext::set_rcv_wnd(u_int16_t rcv_wnd) {
//    rcv_wnd_ = rcv_wnd;
//}
//
//TimeoutEvent* SimpleUDPReliabilityContext::get_timeout_event() {
//    return timer_;
//}
//
//void SimpleUDPReliabilityContext::set_timeout_event(TimeoutEvent* e) {
//    timer_ = e;
//}
//
//double SimpleUDPReliabilityContext::get_rto() {
//    return rto_;
//}
//
//void SimpleUDPReliabilityContext::set_rto(double rto) {
//    rto_ = rto;
//}
//
//u_int32_t SimpleUDPReliabilityContext::get_duplicate_ack_number() {
//    return duplicate_ack_number_;
//}
//
//void SimpleUDPReliabilityContext::set_duplicate_ack_number(u_int32_t num) {
//    duplicate_ack_number_ = num;
//}
//
//int SimpleUDPReliabilityContext::get_duplicates() {
//    return duplicates_;
//}
//
//void SimpleUDPReliabilityContext::set_duplicates(int duplicates) {
//    duplicates_ = duplicates;
//}
//
//TCPPacketBuffer& SimpleUDPReliabilityContext::get_receive_window() {
//    return receive_window_;
//}
//
ReceiveEvent* SimpleUDPReliabilityContext::get_receive_event() {
    return receive_event_;
}

void SimpleUDPReliabilityContext::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}
//
//u_int32_t SimpleUDPReliabilityContext::get_echo_reply() {
//    return echo_reply_;
//}
//
//void SimpleUDPReliabilityContext::set_echo_reply(u_int32_t echo_reply) {
//    echo_reply_ = echo_reply;
//}
//
//double SimpleUDPReliabilityContext::get_srtt() {
//    return srtt_;
//}
//
//void SimpleUDPReliabilityContext::set_srtt(double srtt) {
//    srtt_ = srtt;
//}
//
//double SimpleUDPReliabilityContext::get_rttvar() {
//    return rttvar_;
//}
//
//void SimpleUDPReliabilityContext::set_rttvar(double rttvar) {
//    rttvar_ = rttvar;
//}

