#include "events/framework_events/NetworkSendPacketEvent.h"

NetworkSendPacketEvent::NetworkSendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketHolder(packet), FrameworkEvent(socket) {

}

NetworkSendPacketEvent::~NetworkSendPacketEvent() {

}

void NetworkSendPacketEvent::execute(IModule* m) {
    m->imodule_network_send(this);
}
