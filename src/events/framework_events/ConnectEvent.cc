#include "events/framework_events/ConnectEvent.h"

ConnectEvent::ConnectEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s), destination_(0) {
}

ConnectEvent::~ConnectEvent() {
}

void ConnectEvent::execute(IModule* m) {
    m->imodule_library_connect(this);
}

AddressPort* ConnectEvent::get_destination() {
    if (!destination_) {
        string address = get_map()[ADDRESS_STRING];
        int port = atoi(get_map()[PORT_STRING].c_str());
        destination_ = new AddressPort(address, port);
    }
    return destination_;
}