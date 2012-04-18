#include "events/framework_events/SocketEvent.h"

SocketEvent::SocketEvent() : LibraryEvent() {

}

SocketEvent::~SocketEvent() {

}

void SocketEvent::execute(IModule* m) {
    m->imodule_library_socket(this);
}

int SocketEvent::get_domain() {
    return ((struct SocketMessage*) get_buffer())->domain;
}

int SocketEvent::get_type() {
    return ((struct SocketMessage*) get_buffer())->type;
}

int SocketEvent::get_protocol() {
    return ((struct SocketMessage*) get_buffer())->protocol;
}
