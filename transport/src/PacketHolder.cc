#include "PacketHolder.h"

PacketHolder::PacketHolder(WiFuPacket* p) : packet_(p) {
}

PacketHolder::~PacketHolder() {
}

WiFuPacket* PacketHolder::get_packet() {
    assert(packet_);
    return packet_;
}
