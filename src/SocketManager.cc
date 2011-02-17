#include "SocketManager.h"

SocketManager::SocketManager() : RandomNumberSet<uint16_t>() {
	// We cannot have a socket with id 0 because we use 0
	// As the default socket on the front end
	// see wifu_socket() for details.
	insert(0);
}

SocketManager::~SocketManager() {}

SocketManager& SocketManager::instance() {
	static SocketManager instance_;
	return instance_;
}
