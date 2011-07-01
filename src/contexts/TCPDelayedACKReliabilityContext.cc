/* 
 * File:   TCPDelayedACKReliabilityContext.cc
 * Author: leer
 * 
 * Created on June 17, 2011, 2:09 PM
 */

#include "contexts/TCPDelayedACKReliabilityContext.h"

TCPDelayedACKReliabilityContext::TCPDelayedACKReliabilityContext(u_int32_t iss) : TCPTahoeReliabilityContext(iss), delay_timeout_interval_(INITAL_DELAY_TIMEOUT_INTERVAL), delay_count_(DEFAULT_DELAY_COUNT), delay1_(1), delay2_(3), delay3_(3), delay4_(4), l1_threshold_(20), l2_threshold_(200), l3_threshold_(2000) , ack_timer_(0), cur_ack_count_(0){
    set_state(new TCPDelayedACKReliabilityState());
}

TCPDelayedACKReliabilityContext::~TCPDelayedACKReliabilityContext() {

}

TimeoutEvent* TCPDelayedACKReliabilityContext::get_ack_timeout_event() {
    return ack_timer_;
}

void TCPDelayedACKReliabilityContext::set_ack_timeout_event(TimeoutEvent* e) {
    ack_timer_ = e;
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

u_int32_t TCPDelayedACKReliabilityContext::get_l1_threshold(){
    return l1_threshold_;
}

void TCPDelayedACKReliabilityContext::set_l1_threshold(u_int32_t l1){
    l1_threshold_ = l1;
}

u_int32_t TCPDelayedACKReliabilityContext::get_l2_threshold(){
    return l2_threshold_;
}

void TCPDelayedACKReliabilityContext::set_l2_threshold(u_int32_t l2){
    l2_threshold_ = l2;
}

u_int32_t TCPDelayedACKReliabilityContext::get_l3_threshold(){
    return l3_threshold_;
}

void TCPDelayedACKReliabilityContext::set_l3_threshold(u_int32_t l3){
    l3_threshold_ = l3;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay1(){
    return delay1_;
}

void TCPDelayedACKReliabilityContext::set_delay1(u_int16_t delay){
    delay1_ = delay;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay2(){
    return delay2_;
}

void TCPDelayedACKReliabilityContext::set_delay2(u_int16_t delay){
    delay2_ = delay;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay3(){
    return delay3_;
}

void TCPDelayedACKReliabilityContext::set_delay3(u_int16_t delay){
    delay3_ = delay;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay4(){
    return delay4_;
}

void TCPDelayedACKReliabilityContext::set_delay4(u_int16_t delay){
    delay4_ = delay;
}

u_int16_t TCPDelayedACKReliabilityContext::get_cur_ack_count() {
    return cur_ack_count_;
}

void TCPDelayedACKReliabilityContext::set_cur_ack_count(u_int16_t count) {
    cur_ack_count_ = count;
}