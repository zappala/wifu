/* 
 * File:   OutstandingDataContext.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 3:57 PM
 */

#include "contexts/OutstandingDataContext.h"

OutstandingDataContext::OutstandingDataContext(u_int32_t iss) : iss_(iss), snd_nxt_(0), snd_una_(0), snd_max_(0) {
    // TODO: replace this with the MSS tcp header option
    TCPPacket p;
    TCPTimestampOption o;
    p.insert_tcp_header_option(&o);
    set_mss(p.max_data_length());
}

OutstandingDataContext::~OutstandingDataContext() {
}

u_int32_t OutstandingDataContext::get_iss() const {
    return iss_;
}

void OutstandingDataContext::set_iss(u_int32_t iss) {
    iss_ = iss;
}

u_int16_t OutstandingDataContext::get_mss() const {
    return mss_;
}

void OutstandingDataContext::set_mss(u_int16_t mss) {
    mss_ = mss;
}

u_int32_t OutstandingDataContext::get_snd_nxt() const {
    return snd_nxt_;
}

void OutstandingDataContext::set_snd_nxt(u_int32_t snd_nxt) {
    snd_nxt_ = snd_nxt;

    // keep the biggest snd_nxt_ we have ever seen in snd_max_
    if (less_than(snd_max_, snd_nxt_)) {
        snd_max_ = snd_nxt_;
    }
}

u_int32_t OutstandingDataContext::get_snd_una() const {
    return snd_una_;
}

void OutstandingDataContext::set_snd_una(u_int32_t snd_una) {
    snd_una_ = snd_una;
}

u_int32_t OutstandingDataContext::get_num_outstanding() const {
    return get_snd_nxt() - get_snd_una();
}

u_int32_t OutstandingDataContext::get_snd_max() const {
    return snd_max_;
}

