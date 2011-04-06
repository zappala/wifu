#include "SimpleTCPCache.h"
#include "packet/WiFuPacket.h"

SimpleTCPCache::SimpleTCPCache() : Cache(), packet_(0) {

}

WiFuPacket* SimpleTCPCache::get_packet() {
    assert(!is_empty());

    WiFuPacket* p = packet_;
    packet_ = 0;
    return p;
}

void SimpleTCPCache::save_packet(WiFuPacket* p) {
    assert(is_empty());

    packet_ = p;
}

bool SimpleTCPCache::is_empty() {
    return packet_ == 0;
}