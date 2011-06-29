#include "contexts/TCPTahoeIContextContainer.h"

TCPTahoeIContextContainer::TCPTahoeIContextContainer() : BasicIContextContainer(), saved_send_event_(0), saved_close_event_(0), fin_(0) {
    u_int32_t iss = 1;
    set_congestion_control(new TCPTahoeCongestionControlContext(iss));
    set_connection_manager(new ConnectionManagerContext());
    set_reliability(new TCPTahoeReliabilityContext(iss));
}

TCPTahoeIContextContainer::~TCPTahoeIContextContainer() {

}

SendEvent* TCPTahoeIContextContainer::get_saved_send_event() {
    return saved_send_event_;
}

void TCPTahoeIContextContainer::set_saved_send_event(SendEvent* e) {
    saved_send_event_ = e;
}

CloseEvent* TCPTahoeIContextContainer::get_saved_close_event() {
    return saved_close_event_;
}

void TCPTahoeIContextContainer::set_saved_close_event(CloseEvent* e) {
    saved_close_event_ = e;
}

NetworkReceivePacketEvent* TCPTahoeIContextContainer::get_saved_fin() {
    return fin_;
}

void TCPTahoeIContextContainer::set_saved_fin(NetworkReceivePacketEvent* e) {
    fin_ = e;
}