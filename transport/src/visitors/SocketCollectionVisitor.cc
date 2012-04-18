#include "visitors/SocketCollectionVisitor.h"

SocketCollectionVisitor::SocketCollectionVisitor() : Visitor(), socket_(0) {}

SocketCollectionVisitor::~SocketCollectionVisitor() {}

void SocketCollectionVisitor::set_socket(Socket* s) {
	socket_ = s;
}

Socket* SocketCollectionVisitor::get_socket() {
	return socket_;
}

bool SocketCollectionVisitor::stop() {
	return socket_ != NULL;
}
