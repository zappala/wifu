#include "AlreadyBoundToAddressPortVisitor.h"

AlreadyBoundToAddressPortVisitor::AlreadyBoundToAddressPortVisitor(AddressPort* ap) : is_bound_(false), ap_(ap) {}

AlreadyBoundToAddressPortVisitor::~AlreadyBoundToAddressPortVisitor() {}

void AlreadyBoundToAddressPortVisitor::visit(Socket* socket) {
	if(socket->get_local_address_port()->operator ==(*ap_)) {
		is_bound_ = true;
	}
}

bool AlreadyBoundToAddressPortVisitor::stop() {
	return is_bound();
}

bool AlreadyBoundToAddressPortVisitor::is_bound() {
	return is_bound_;
}
