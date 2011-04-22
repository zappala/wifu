#include "events/ReceiveBufferNotFullEvent.h"

ReceiveBufferNotFullEvent::ReceiveBufferNotFullEvent(Socket* s) : Event(s) {

}

ReceiveBufferNotFullEvent::~ReceiveBufferNotFullEvent() {

}

void ReceiveBufferNotFullEvent::execute(IModule* m) {
    m->imodule_receive_buffer_not_full(this);
}
