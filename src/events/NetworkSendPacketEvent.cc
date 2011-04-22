#include "events/NetworkSendPacketEvent.h"

NetworkSendPacketEvent::NetworkSendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

NetworkSendPacketEvent::~NetworkSendPacketEvent() {}

void NetworkSendPacketEvent::execute(IModule* m) {
	m->imodule_network_send(this);
}
