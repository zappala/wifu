
#include "states/PassThroughRateLimiter.h"

/*Simple pass-through placeholder for rate limiters.*/
void PassThroughRateLimiter::send_packet(Context* c, SendPacketEvent* e) {
    NetworkSendPacketEvent* network_send_event = new NetworkSendPacketEvent(e->get_socket(), e->get_packet());
    Dispatcher::instance().enqueue(network_send_event);
}