#include "events/protocol_events/SendBufferNotFullEvent.h"

SendBufferNotFullEvent::SendBufferNotFullEvent(Socket* s) : ProtocolEvent(s) {

}

void SendBufferNotFullEvent::execute(IModule* m) {
    m->imodule_send_buffer_not_full(this);
}
