#include "events/framework_events/SendEvent.h"

SendEvent::SendEvent() : DataEvent() {
    
}

void SendEvent::execute(IModule* m) {
    m->imodule_library_send(this);
}

unsigned char * SendEvent::get_data() {
    return get_buffer() + sizeof(struct SendToMessage);
}