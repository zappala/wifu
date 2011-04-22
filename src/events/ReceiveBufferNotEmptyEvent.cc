#include "events/ReceiveBufferNotEmptyEvent.h"

ReceiveBufferNotEmptyEvent::ReceiveBufferNotEmptyEvent(Socket* s) : Event(s) {

}

void ReceiveBufferNotEmptyEvent::execute(IModule* m) {
    m->imodule_receive_buffer_not_empty(this);
}
