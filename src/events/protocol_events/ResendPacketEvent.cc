#include "events/protocol_events/ResendPacketEvent.h"

ResendPacketEvent::ResendPacketEvent(Socket* s, WiFuPacket* p) : PacketHolder(p), ProtocolEvent(s) {
}

void ResendPacketEvent::execute(IModule* m) {
    m->imodule_resend(this);
}
