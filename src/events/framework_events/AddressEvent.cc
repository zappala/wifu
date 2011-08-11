#include "events/framework_events/AddressEvent.h"

AddressEvent::AddressEvent() : LibraryEvent(), address_(0) {

}

AddressEvent::~AddressEvent() {

}

AddressPort* AddressEvent::get_address() {
    if (!address_) {
        struct sockaddr_in* d = &(((struct AddressMessage*) get_buffer())->addr);
        address_ = new AddressPort(d);
    }
    return address_;
}
