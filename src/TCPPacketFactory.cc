#include "TCPPacketFactory.h"

TCPPacketFactory::TCPPacketFactory() : PacketFactory() {
    for(int i = 0; i < 5000; ++i) {
        packets_.push_back(new TCPPacket());
    }
}

WiFuPacket* TCPPacketFactory::create() {

    if(packets_.empty()) {
        return new TCPPacket();
    }

    p_ = packets_.back();
    packets_.pop_back();
    return p_;
}
