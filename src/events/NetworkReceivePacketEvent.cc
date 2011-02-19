#include "NetworkReceivePacketEvent.h"

NetworkReceivePacketEvent::NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

NetworkReceivePacketEvent::~NetworkReceivePacketEvent() {}

void NetworkReceivePacketEvent::execute(IModule* m) {
	m->network_receive(this);
}
