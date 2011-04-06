#include "contexts/IContextContainer.h"

IContextContainer::IContextContainer() : saved_send_event_(0) {
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