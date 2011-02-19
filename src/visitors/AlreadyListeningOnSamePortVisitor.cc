#include "visitors/AlreadyListeningOnSamePortVisitor.h"

AlreadyListeningOnSamePortVisitor::AlreadyListeningOnSamePortVisitor(u_int16_t port) : is_listening_(false), port_(port) {}

AlreadyListeningOnSamePortVisitor::~AlreadyListeningOnSamePortVisitor() {}

void AlreadyListeningOnSamePortVisitor::visit(Socket* socket) {
	if( socket->is_passive() &&
		socket->get_local_address_port()->get_port() == port_) {

		is_listening_ = true;
	}
}

bool AlreadyListeningOnSamePortVisitor::stop() {
	return is_listening();
}

bool AlreadyListeningOnSamePortVisitor::is_listening() {
	return is_listening_;
}
