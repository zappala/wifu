#include "events/protocol_events/ConnectionEstablishedEvent.h"

ConnectionEstablishedEvent::ConnectionEstablishedEvent(AcceptEvent* e, Socket* new_socket) :
						ProtocolEvent(e->get_socket()),
						new_socket_(new_socket),
						event_(e) {}

ConnectionEstablishedEvent::~ConnectionEstablishedEvent() {}

void ConnectionEstablishedEvent::execute(IModule* m) {
    m->imodule_connection_established(this);
}

Socket* ConnectionEstablishedEvent::get_new_socket() {
    return new_socket_;
}

AcceptEvent* ConnectionEstablishedEvent::get_accept_event() {
    return event_;
}
