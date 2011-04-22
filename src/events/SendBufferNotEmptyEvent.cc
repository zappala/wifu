#include "events/SendBufferNotEmptyEvent.h"

SendBufferNotEmptyEvent::SendBufferNotEmptyEvent(Socket* s) : Event(s) {

}

void SendBufferNotEmptyEvent::execute(IModule* m) {
    m->imodule_send_buffer_not_empty(this);
}
