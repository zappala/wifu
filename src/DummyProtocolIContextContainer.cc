#include "DummyProtocolIContextContainer.h"

DummyProtocolIContextContainer::DummyProtocolIContextContainer() : TCPTahoeIContextContainer(), receive_event_(0) {

}

DummyProtocolIContextContainer::~DummyProtocolIContextContainer() {

}

ReceiveEvent* DummyProtocolIContextContainer::get_receive_event() const {
    return receive_event_;
}

void DummyProtocolIContextContainer::set_receive_event(ReceiveEvent* e) {
    receive_event_ = e;
}
