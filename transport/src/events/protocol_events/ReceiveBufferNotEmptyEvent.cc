#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"

ReceiveBufferNotEmptyEvent::ReceiveBufferNotEmptyEvent(Socket* s) : ProtocolEvent(s) {

}

ReceiveBufferNotEmptyEvent::~ReceiveBufferNotEmptyEvent() {
    
}

void ReceiveBufferNotEmptyEvent::execute(IModule* m) {
    m->imodule_receive_buffer_not_empty(this);
}
