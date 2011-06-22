#include "contexts/TCPTahoeIContextContainer.h"

TCPTahoeIContextContainer::TCPTahoeIContextContainer() : saved_send_event_(0), saved_close_event_(0), fin_(0) {
    u_int32_t iss = 1;
    connection_manager_ = new ConnectionManagerContext();
    congestion_control_ = new TCPTahoeCongestionControlContext(iss);
    reliability_ = new TCPTahoeReliabilityContext(iss);
    //Just testing some stuff, Randy...go ahead and delete this without feeling the need to kill me...
    //reliability_ = new TCPDelayedACKReliabilityContext(iss);
}

TCPTahoeIContextContainer::~TCPTahoeIContextContainer() {

}

IContext* TCPTahoeIContextContainer::get_connection_manager() {
    return connection_manager_;
}

IContext* TCPTahoeIContextContainer::get_congestion_control() {
    return congestion_control_;
}

IContext* TCPTahoeIContextContainer::get_reliability() {
    return reliability_;
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