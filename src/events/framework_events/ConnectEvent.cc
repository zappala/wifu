#include "events/framework_events/ConnectEvent.h"

ConnectEvent::ConnectEvent() : AddressEvent() {
    
}

ConnectEvent::~ConnectEvent() {
}

void ConnectEvent::execute(IModule* m) {
    m->imodule_library_connect(this);
}