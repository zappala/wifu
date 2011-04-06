#include "events/ReceiveBufferNotEmpty.h"

ReceiveBufferNotEmpty::ReceiveBufferNotEmpty(Socket* s) : Event(s) {

}

void ReceiveBufferNotEmpty::execute(IModule* m) {
    m->receive_buffer_not_empty(this);
}
