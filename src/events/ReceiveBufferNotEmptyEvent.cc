#include "events/ReceiveBufferNotEmptyEvent.h"

ReceiveBufferNotEmptyEvent::ReceiveBufferNotEmptyEvent(Socket* s) : Event(s) {

}

void ReceiveBufferNotEmptyEvent::execute(IModule* m) {
    m->receive_buffer_not_empty(this);
}
