#include "events/SendBufferNotFullEvent.h"

SendBufferNotFullEvent::SendBufferNotFullEvent(Socket* s) : Event(s) {

}

void SendBufferNotFullEvent::execute(IModule* m) {
    m->imodule_send_buffer_not_full(this);
}
