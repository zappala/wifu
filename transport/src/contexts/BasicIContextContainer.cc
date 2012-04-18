#include "contexts/BasicIContextContainer.h"

BasicIContextContainer::BasicIContextContainer() : congestion_control_(0), connection_manager_(0), reliability_(0) {

}

BasicIContextContainer::~BasicIContextContainer() {

}

IContext* BasicIContextContainer::get_connection_manager() const {
    return connection_manager_;
}

void BasicIContextContainer::set_connection_manager(IContext* connection_manager) {
    connection_manager_ = connection_manager;
}

IContext* BasicIContextContainer::get_congestion_control() const {
    return congestion_control_;
}

void BasicIContextContainer::set_congestion_control(IContext* congestion_control) {
    congestion_control_ = congestion_control;
}

IContext* BasicIContextContainer::get_reliability() const {
    return reliability_;
}

void BasicIContextContainer::set_reliability(IContext* reliability) {
    reliability_ = reliability;
}
