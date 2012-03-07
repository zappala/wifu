#include "contexts/SimpleTCPIContextContainer.h"

SimpleTCPIContextContainer::SimpleTCPIContextContainer() : saved_send_event_(0), saved_receive_event_(0), close_event_(0), fin_(0), echo_reply_(0) {
    reliability_ = new SimpleTCPReliabilityContext();
    cc_ = new SimpleTCPCongestionControlContext();
    cm_ = new ConnectionManagerContext();
}

SimpleTCPIContextContainer::SimpleTCPIContextContainer(SimpleTCPIContextContainer const& other) {

}

IContext* SimpleTCPIContextContainer::get_congestion_control() {
    return cc_;
}

IContext* SimpleTCPIContextContainer::get_reliability() {
    return reliability_;
}

IContext* SimpleTCPIContextContainer::get_connection_manager() {
    return cm_;
}

SendEvent* SimpleTCPIContextContainer::get_saved_send_event() {
    return saved_send_event_;
}

void SimpleTCPIContextContainer::set_saved_send_event(SendEvent* e) {
    saved_send_event_ = e;
}

ReceiveEvent* SimpleTCPIContextContainer::get_saved_receive_event() {
    return saved_receive_event_;
}

void SimpleTCPIContextContainer::set_saved_receive_event(ReceiveEvent* e) {
    saved_receive_event_ = e;
}

CloseEvent* SimpleTCPIContextContainer::get_saved_close_event() {
    return close_event_;
}

void SimpleTCPIContextContainer::set_saved_close_event(CloseEvent* e) {
    close_event_ = e;
}

NetworkReceivePacketEvent* SimpleTCPIContextContainer::get_fin() {
    return fin_;
}

void SimpleTCPIContextContainer::set_fin(NetworkReceivePacketEvent* e) {
    fin_ = e;
}

u_int32_t SimpleTCPIContextContainer::get_echo_reply() {
    return echo_reply_;
}

void SimpleTCPIContextContainer::set_echo_reply(u_int32_t echo_reply) {
    echo_reply_ = echo_reply;
}