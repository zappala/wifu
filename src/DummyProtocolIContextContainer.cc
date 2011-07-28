#include "DummyProtocolIContextContainer.h"

DummyProtocolIContextContainer::DummyProtocolIContextContainer() : TCPTahoeIContextContainer(), receive_event_(0), receive_index_(0) {

}

DummyProtocolIContextContainer::~DummyProtocolIContextContainer() {

}

ReceiveEvent* DummyProtocolIContextContainer::get_receive_event() const {
    return receive_event_;
}

void DummyProtocolIContextContainer::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}

int DummyProtocolIContextContainer::get_receive_index() const {
    return receive_index_;
}

void DummyProtocolIContextContainer::set_receive_index(int receive_index) {
    receive_index_ = receive_index;
}
