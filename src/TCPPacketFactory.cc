#include "TCPPacketFactory.h"

WiFuPacket* TCPPacketFactory::create() {
    return new TCPPacket();
}
