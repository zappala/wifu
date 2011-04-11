#include "PortManager.h"

PortManager::PortManager() : IPortManager(), RandomNumberSet<uint16_t>() {}

PortManager::~PortManager() {}

PortManager& PortManager::instance() {
	static PortManager instance_;
	return instance_;
}

u_int16_t PortManager::get() {
    return this->RandomNumberSet<u_int16_t>::get();
}

void PortManager::remove(u_int16_t port) {
    this->RandomNumberSet<u_int16_t>::remove(port);
}

void PortManager::reset() {
    this->RandomNumberSet<u_int16_t>::reset();
}