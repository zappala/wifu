#include "events/framework_events/GetSockOptResponseEvent.h"

GetSockOptResponseEvent::GetSockOptResponseEvent() {

}

GetSockOptResponseEvent::~GetSockOptResponseEvent() {

}

void GetSockOptResponseEvent::set_optval(void* optval, socklen_t optlen) {
    ((struct GetSockOptResponseMessage*) get_buffer())->optlen = optlen;
    memcpy(get_buffer() + sizeof (struct GetSockOptResponseMessage), optval, optlen);
    set_length(sizeof (struct GetSockOptResponseMessage) + optlen);
}