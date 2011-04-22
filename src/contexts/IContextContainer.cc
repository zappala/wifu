#include "contexts/IContextContainer.h"

IContextContainer::IContextContainer() : saved_send_event_(0), saved_receive_event_(0), close_event_(0), fin_(0) {
    reliability_ = new ReliabilityContext();
    cc_ = new CongestionControlContext();
    cm_ = new ConnectionManagerContext();
}

IContextContainer::IContextContainer(IContextContainer const& other) {

}

IContext* IContextContainer::get_congestion_control() {
    return cc_;
}

IContext* IContextContainer::get_reliability() {
    return reliability_;
}

IContext* IContextContainer::get_connection_manager() {
    return cm_;
}

SendEvent* IContextContainer::get_saved_send_event() {
    return saved_send_event_;
}

void IContextContainer::set_saved_send_event(SendEvent* e) {
    saved_send_event_ = e;
}

ReceiveEvent* IContextContainer::get_saved_receive_event() {
    return saved_receive_event_;
}

void IContextContainer::set_saved_receive_event(ReceiveEvent* e) {
    saved_receive_event_ = e;
}

CloseEvent* IContextContainer::get_saved_close_event() {
    return close_event_;
}

void IContextContainer::set_saved_close_event(CloseEvent* e) {
    close_event_ = e;
}

NetworkReceivePacketEvent* IContextContainer::get_fin() {
    return fin_;
}

void IContextContainer::set_fin(NetworkReceivePacketEvent* e) {
    fin_ = e;
}