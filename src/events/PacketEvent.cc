#include "PacketEvent.h"

PacketEvent::PacketEvent(Socket* socket, WiFuPacket* p) : Event(socket), packet_(p) {}

PacketEvent::~PacketEvent() {}

WiFuPacket* PacketEvent::get_packet() {
	return packet_;
}
