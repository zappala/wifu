#include "events/protocol_events/ResendPacketEvent.h"

ResendPacketEvent::ResendPacketEvent(Socket* s, WiFuPacket* p, ResendReason reason) : reason_(reason), PacketHolder(p), ProtocolEvent(s) {
}

ResendPacketEvent::ResendPacketEvent(Socket* s, ResendReason reason) : reason_(reason), PacketHolder(0), ProtocolEvent(s) {

}

ResendPacketEvent::~ResendPacketEvent() {

}

void ResendPacketEvent::execute(IModule* m) {
    m->imodule_resend(this);
}

ResendReason ResendPacketEvent::get_reason() const {
    return reason_;
}

void ResendPacketEvent::set_reason(ResendReason reason) {
    reason_ = reason;
}
