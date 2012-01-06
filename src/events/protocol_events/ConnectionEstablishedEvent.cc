#include "events/protocol_events/ConnectionEstablishedEvent.h"

ConnectionEstablishedEvent::ConnectionEstablishedEvent(Socket* listening_socket, Socket* new_socket) :
						ProtocolEvent(listening_socket),
						new_socket_(new_socket){

}

ConnectionEstablishedEvent::~ConnectionEstablishedEvent() {}

void ConnectionEstablishedEvent::execute(IModule* m) {
    m->imodule_connection_established(this);
}

Socket* ConnectionEstablishedEvent::get_new_socket() {
    return new_socket_;
}