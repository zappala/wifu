#include "events/protocol_events/ResendPacketEvent.h"

ResendPacketEvent::ResendPacketEvent(Socket* s, WiFuPacket* p) : PacketHolder(p), ProtocolEvent(s) {
}

ResendPacketEvent::ResendPacketEvent(Socket* s) : PacketHolder(0), ProtocolEvent(s) {

}

ResendPacketEvent::~ResendPacketEvent() {

}

void ResendPacketEvent::execute(IModule* m) {
    m->imodule_resend(this);
}
