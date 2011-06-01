#include "PacketHolder.h"

PacketHolder::PacketHolder(WiFuPacket* p) : packet_(p) {}

PacketHolder::~PacketHolder() {}

WiFuPacket* PacketHolder::get_packet() {
	return packet_;
}
