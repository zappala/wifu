#include "visitors/SocketCollectionGetByLocalAddressPortVisitor.h"

SocketCollectionGetByLocalAddressPortVisitor::SocketCollectionGetByLocalAddressPortVisitor(AddressPort* local) : local_(local) {
}

SocketCollectionGetByLocalAddressPortVisitor::~SocketCollectionGetByLocalAddressPortVisitor() {
}

void SocketCollectionGetByLocalAddressPortVisitor::visit(Socket* s) {
    if ((*s->get_local_address_port()) == (*local_) && (*s->get_remote_address_port()) == (*AddressPort::default_address_port())) {
        set_socket(s);
    }
}
