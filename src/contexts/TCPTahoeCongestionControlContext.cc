#include "contexts/TCPTahoeCongestionControlContext.h"

TCPTahoeCongestionControlContext::TCPTahoeCongestionControlContext(u_int32_t iss) : OutstandingDataContext(iss) {
    set_state(new DummyCongestionController());
}

TCPTahoeCongestionControlContext::~TCPTahoeCongestionControlContext() {

}

u_int32_t TCPTahoeCongestionControlContext::get_snd_wnd() const {
    return snd_wnd_;
}

void TCPTahoeCongestionControlContext::set_snd_wnd(u_int32_t snd_wnd) {
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