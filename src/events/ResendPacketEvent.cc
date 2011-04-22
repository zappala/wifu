#include "events/ResendPacketEvent.h"

ResendPacketEvent::ResendPacketEvent(Socket* s, WiFuPacket* p) : PacketEvent(s, p) {
}

void ResendPacketEvent::execute(IModule* m) {
    m->imodule_resend(this);
}
