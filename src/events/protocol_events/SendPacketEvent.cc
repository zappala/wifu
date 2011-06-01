#include "events/protocol_events/SendPacketEvent.h"

SendPacketEvent::SendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketHolder(packet) {}

void SendPacketEvent::execute(IModule* m) {
	m->imodule_send(this);
}
