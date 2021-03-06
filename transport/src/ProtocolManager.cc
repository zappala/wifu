#include "ProtocolManager.h"

ProtocolManager::ProtocolManager() {
	mutex_.init(1);
}

ProtocolManager::ProtocolManager(ProtocolManager const&) {}

ProtocolManager& ProtocolManager::operator=(ProtocolManager const&) {}

ProtocolManager::~ProtocolManager() {}

ProtocolManager& ProtocolManager::instance() {
	static ProtocolManager instance_;
	return instance_;
}

bool ProtocolManager::is_supported(int domain, int type, int protocol) {
	// TODO: support checking the three-tuple:
	// TODO: domain, type, protocol
	// TODO: It is possible that the protocol will be 0,
	// TODO: in which case it is up to this class to
	// TODO: determine whether or not it is supported
	// TODO: This means that the class will need to map the
	// TODO: three-tuple to a protocol
	mutex_.wait();
	bool value = contains(protocol);
	mutex_.post();
	return value;
}

void ProtocolManager::support(int protocol) {
	mutex_.wait();
	insert(protocol);
	mutex_.post();
}
