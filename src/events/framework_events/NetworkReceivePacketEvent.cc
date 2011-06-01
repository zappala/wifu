#include "events/framework_events/NetworkReceivePacketEvent.h"

NetworkReceivePacketEvent::NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketHolder(packet) {}

NetworkReceivePacketEvent::~NetworkReceivePacketEvent() {}

void NetworkReceivePacketEvent::execute(IModule* m) {
	m->imodule_network_receive(this);
}
