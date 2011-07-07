#include "events/framework_events/SocketEvent.h"

SocketEvent::SocketEvent(gcstring& message, gcstring& file, Socket* socket) : LibraryEvent(message, file, socket) {

}

SocketEvent::~SocketEvent() {

}

void SocketEvent::execute(IModule* m) {
    m->imodule_library_socket(this);
}
