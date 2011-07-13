#include "events/framework_events/SocketEvent.h"

SocketEvent::SocketEvent(gcstring_map& m, gcstring& file, Socket* socket) : LibraryEvent(m, file, socket) {

}

SocketEvent::~SocketEvent() {

}

void SocketEvent::execute(IModule* m) {
    m->imodule_library_socket(this);
}
