/* 
 * File:   OutstandingDataContext.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 3:57 PM
 */

#include "contexts/OutstandingDataContext.h"
#include "Math.h"

OutstandingDataContext::OutstandingDataContext(u_int32_t iss) : iss_(iss), snd_nxt_(0), snd_una_(0), snd_max_(0) {
}

OutstandingDataContext::OutstandingDataContext(const OutstandingDataContext& orig) {
    iss_ = orig.iss_;
    snd_nxt_ = orig.snd_nxt_;
    snd_una_ = orig.snd_una_;
}

OutstandingDataContext::~OutstandingDataContext() {
}

u_int32_t OutstandingDataContext::get_iss() const {
    return iss_;
}

void OutstandingDataContext::set_iss(u_int32_t iss) {
    iss_ = iss;
}

u_int32_t OutstandingDataContext::get_snd_nxt() const {
    return snd_nxt_;
}

void OutstandingDataContext::set_snd_nxt(u_int32_t snd_nxt) {
    snd_nxt_ = snd_nxt;

    // keep the biggest snd_nxt_ we have ever seen in snd_max_
    if(less_than(snd_max_, snd_nxt_)) {
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

