#include "contexts/TCPTahoeCongestionControlContext.h"

TCPTahoeCongestionControlContext::TCPTahoeCongestionControlContext(u_int32_t iss) : OutstandingDataContext(iss), probe_timer_(0), probe_timer_duration_(INITIAL_PROBE_TIMEOUT_DURATION), cwnd_(USHRT_MAX), ssthreashold_(USHRT_MAX) {
    // TODO: replace this with the MSS tcp header option
    TCPPacket p;
    TCPTimestampOption o;
    p.insert_tcp_header_option(&o);
    set_mss(p.max_data_length());
    set_state(new SlowStart());
}

TCPTahoeCongestionControlContext::~TCPTahoeCongestionControlContext() {

}

u_int16_t TCPTahoeCongestionControlContext::get_snd_wnd() const {
    return snd_wnd_;
}

void TCPTahoeCongestionControlContext::set_snd_wnd(u_int16_t snd_wnd) {
    snd_wnd_ = snd_wnd;
}

u_int32_t TCPTahoeCongestionControlContext::get_snd_wnd1() const {
    return snd_wnd1_;
}

void TCPTahoeCongestionControlContext::set_snd_wnd1(u_int32_t snd_wnd1) {
    snd_wnd1_ = snd_wnd1;
}

u_int32_t TCPTahoeCongestionControlContext::get_snd_wnd2() const {
    return snd_wnd2_;
}

void TCPTahoeCongestionControlContext::set_snd_wnd2(u_int32_t snd_wnd2) {
    snd_wnd2_ = snd_wnd2;
}

TimeoutEvent* TCPTahoeCongestionControlContext::get_probe_timer() const {
    return probe_timer_;
}

void TCPTahoeCongestionControlContext::set_probe_timer(TimeoutEvent* e) {
    probe_timer_ = e;
}

int TCPTahoeCongestionControlContext::get_probe_timer_duration() const {
    return probe_timer_duration_;
}

void TCPTahoeCongestionControlContext::set_probe_timer_duration(int duration) {
    probe_timer_duration_ = duration;
}

u_int16_t TCPTahoeCongestionControlContext::get_cwnd() const {
    return cwnd_;
}

void TCPTahoeCongestionControlContext::set_cwnd(u_int16_t cwnd) {
    cwnd_ = cwnd;
}

u_int16_t TCPTahoeCongestionControlContext::get_ssthreshold() const {
    return ssthreashold_;
}

void TCPTahoeCongestionControlContext::set_ssthreshold(u_int16_t ssthreashold) {
    ssthreashold_ = ssthreashold;
}

u_int16_t TCPTahoeCongestionControlContext::get_max_allowed_to_send() const {
    return min(get_snd_wnd(), get_cwnd());
}

u_int16_t TCPTahoeCongestionControlContext::get_mss() const {
    return mss_;
}

void TCPTahoeCongestionControlContext::set_mss(u_int16_t mss) {
    mss_ = mss;
}