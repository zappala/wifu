#include "SimpleUDPContainer.h"

SimpleUDPContainer::SimpleUDPContainer() : receive_event_(0) {

}

SimpleUDPContainer::~SimpleUDPContainer() {

}

void SimpleUDPContainer::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}

ReceiveEvent* SimpleUDPContainer::get_receive_event() const {
    return receive_event_;
}

packet_queue& SimpleUDPContainer::get_packet_queue() {
    return packet_queue_;
}
