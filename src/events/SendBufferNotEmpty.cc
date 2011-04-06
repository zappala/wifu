#include "events/SendBufferNotEmpty.h"

SendBufferNotEmpty::SendBufferNotEmpty(Socket* s) : Event(s) {

}

void SendBufferNotEmpty::execute(IModule* m) {
    m->send_buffer_not_empty(this);
}
