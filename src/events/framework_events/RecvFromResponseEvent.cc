#include "events/framework_events/RecvFromResponseEvent.h"

RecvFromResponseEvent::RecvFromResponseEvent() : AddressResponseEvent() {

}

RecvFromResponseEvent::~RecvFromResponseEvent() {

}

void RecvFromResponseEvent::set_return_buffer(unsigned char* buffer, size_t length) {
    assert(length >= 0);
    memcpy(get_buffer() + sizeof(struct RecvFromResponseMessage), buffer, length);
    set_return_value(length);
    set_length(sizeof(struct RecvFromResponseMessage) + length);
}