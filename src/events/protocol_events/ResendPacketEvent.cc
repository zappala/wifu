#include "events/protocol_events/ResendPacketEvent.h"

ResendPacketEvent::ResendPacketEvent(Socket* s, WiFuPacket* p) : PacketHolder(p) {
}

void ResendPacketEvent::execute(IModule* m) {
    m->imodule_resend(this);
}
