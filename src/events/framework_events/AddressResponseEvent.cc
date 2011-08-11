#include "events/framework_events/AddressResponseEvent.h"

AddressResponseEvent::AddressResponseEvent() {

}

AddressResponseEvent::~AddressResponseEvent() {

}

void AddressResponseEvent::set_addr(struct sockaddr_in* addr, socklen_t addr_len) {
    struct AddressResponseMessage* s = (struct AddressResponseMessage*) get_buffer();
    s->addr_len = addr_len;
    memcpy(&(s->addr), addr, addr_len);
    set_length(sizeof (struct AddressResponseMessage));
}
