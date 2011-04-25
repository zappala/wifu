
#include "states/PassThroughRateLimiter.h"

/*Simple pass-through placeholder for rate limiters.*/
void PassThroughRateLimiter::send_packet(Context* c, SendPacketEvent* e)
{
    NetworkSendPacketEvent* netSendEvent = new NetworkSendPacketEvent(e->get_socket(), e->get_packet());
    Dispatcher::instance().enqueue(netSendEvent);
}