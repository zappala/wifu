#include "events/protocol_events/ConnectionInitiatedEvent.h"

ConnectionInitiatedEvent::ConnectionInitiatedEvent( Socket* listening_socket,
                                                    Socket* new_socket,
                                                    ListenEvent* listen_event) :
                                                    ProtocolEvent(listening_socket),
                                                    new_socket_(new_socket),
                                                    listen_event_(listen_event) {

}

ConnectionInitiatedEvent::~ConnectionInitiatedEvent() {

}

void ConnectionInitiatedEvent::execute(IModule* m) {
    m->imodule_connection_initiated(this);
}

Socket* ConnectionInitiatedEvent::get_new_socket() {
    return new_socket_;
}

ListenEvent* ConnectionInitiatedEvent::get_listen_event() {
    return listen_event_;
}
