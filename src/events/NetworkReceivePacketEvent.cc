#include "events/NetworkReceivePacketEvent.h"

NetworkReceivePacketEvent::NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

NetworkReceivePacketEvent::~NetworkReceivePacketEvent() {}

void NetworkReceivePacketEvent::execute(IModule* m) {
	m->imodule_network_receive(this);
}
