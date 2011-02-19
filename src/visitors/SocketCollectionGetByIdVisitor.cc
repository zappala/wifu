#include "SocketCollectionGetByIdVisitor.h"

SocketCollectionGetByIdVisitor::SocketCollectionGetByIdVisitor(int id) : id_(id) {}

SocketCollectionGetByIdVisitor::~SocketCollectionGetByIdVisitor() {}

void SocketCollectionGetByIdVisitor::visit(Socket* s) {
	if(s->get_socket_id() == id_) {
		set_socket(s);
	}
}
