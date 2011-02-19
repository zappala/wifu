#include "SocketCollectionGetByLocalAddressPortVisitor.h"

SocketCollectionGetByLocalAddressPortVisitor::SocketCollectionGetByLocalAddressPortVisitor(AddressPort* local) : local_(local) {}

SocketCollectionGetByLocalAddressPortVisitor::~SocketCollectionGetByLocalAddressPortVisitor() {}

void SocketCollectionGetByLocalAddressPortVisitor::visit(Socket* s) {
	if ((*s->get_local_address_port()) == (*local_)) {
		set_socket(s);
	}
}
