#include "contexts/TCPTahoeIContextContainer.h"

TCPTahoeIContextContainer::TCPTahoeIContextContainer() : BasicIContextContainer(), saved_send_event_(0), saved_close_event_(0), fin_(0), saved_accept_event_(0) {
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
    if (!e) {
        saved_close_event_ = e;
        return;
    }
    
    // Because the back end translator reuses LibraryEvent objects,
    // and close immediately returns, we must copy the close event for our purposes
    // so it doesn't become corrupt by another socket using it
    saved_close_event_ = new CloseEvent();
    saved_close_event_->set_socket(e->get_socket());
    saved_close_event_->save_buffer(e->get_buffer(), e->get_buffer_length());
}

NetworkReceivePacketEvent* TCPTahoeIContextContainer::get_saved_fin() {
    return fin_;
}

void TCPTahoeIContextContainer::set_saved_fin(NetworkReceivePacketEvent* e) {
    fin_ = e;
}

BackLog* TCPTahoeIContextContainer::get_back_log() {
    return back_log_;
}

void TCPTahoeIContextContainer::set_back_log(BackLog* bl) {
    back_log_ = bl;
}

AcceptEvent* TCPTahoeIContextContainer::get_saved_accept_event() {
    return saved_accept_event_;
}

void TCPTahoeIContextContainer::set_saved_accept_event(AcceptEvent* e) {
    saved_accept_event_ = e;
}