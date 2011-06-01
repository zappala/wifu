#include "events/protocol_events/SendBufferNotEmptyEvent.h"

SendBufferNotEmptyEvent::SendBufferNotEmptyEvent(Socket* s) : ProtocolEvent(s) {

}

void SendBufferNotEmptyEvent::execute(IModule* m) {
    m->imodule_send_buffer_not_empty(this);
}
