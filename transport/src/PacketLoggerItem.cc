#include "PacketLoggerItem.h"

PacketLoggerItem::PacketLoggerItem(WiFuPacket* p) : packet_(p) {
    gettimeofday(&time_, NULL);
}

PacketLoggerItem::~PacketLoggerItem() {

}

WiFuPacket* PacketLoggerItem::get_packet() const {
    return packet_;
}

struct timeval* PacketLoggerItem::get_time() {
    return &time_;
}
