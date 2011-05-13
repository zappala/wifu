#include "contexts/TCPTahoeIContextContainer.h"

TCPTahoeIContextContainer::TCPTahoeIContextContainer() : saved_send_event_(0), saved_close_event_(0) {
    connection_manager_ = new ConnectionManagerContext();
    congestion_control_ = new TCPTahoeCongestionControlContext();
    reliability_ = new TCPTahoeReliabilityContext();
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