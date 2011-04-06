#include "events/SendBufferNotFull.h"

SendBufferNotFull::SendBufferNotFull(Socket* s) : Event(s) {

}

void SendBufferNotFull::execute(IModule* m) {
    m->send_buffer_not_full(this);
}
