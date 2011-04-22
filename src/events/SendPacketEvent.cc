#include "events/SendPacketEvent.h"

SendPacketEvent::SendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

void SendPacketEvent::execute(IModule* m) {
	m->imodule_send(this);
}
