#include "events/framework_events/ReceiveEvent.h"

ReceiveEvent::ReceiveEvent() : DataEvent() {
    
}

ReceiveEvent::~ReceiveEvent() {
    
}

void ReceiveEvent::execute(IModule* m) {
    m->imodule_library_receive(this);
}