#include "SimpleTCPCache.h"
#include "packet/WiFuPacket.h"

SimpleTCPCache::SimpleTCPCache() : Cache(), packet_(0) {

}

WiFuPacket* SimpleTCPCache::get_packet() {
    WiFuPacket* p = packet_;
    return p;
}

void SimpleTCPCache::save_packet(WiFuPacket* p) {
    packet_ = p;
}

bool SimpleTCPCache::is_empty() {
    return packet_ == 0;
}