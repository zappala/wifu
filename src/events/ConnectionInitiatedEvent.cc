#include "events/ConnectionInitiatedEvent.h"

ConnectionInitiatedEvent::ConnectionInitiatedEvent( Socket* listening_socket,
                                                    Socket* new_socket) :
                                                    Event(listening_socket),
                                                    new_socket_(new_socket) {

}

ConnectionInitiatedEvent::~ConnectionInitiatedEvent() {

}

void ConnectionInitiatedEvent::execute(IModule* m) {
    m->imodule_connection_initiated(this);
}

Socket* ConnectionInitiatedEvent::get_new_socket() {
    return new_socket_;
}
