#include "PortManager.h"

PortManager::PortManager() : RandomNumberSet<uint16_t>() {}

PortManager::~PortManager() {}

PortManager& PortManager::instance() {
	static PortManager instance_;
	return instance_;
}
