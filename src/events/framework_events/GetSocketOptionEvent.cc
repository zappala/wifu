#include "events/framework_events/GetSocketOptionEvent.h"

GetSocketOptionEvent::GetSocketOptionEvent(string& message, string& file, Socket* s) : SocketOptionEvent(message, file, s) {

}

GetSocketOptionEvent::~GetSocketOptionEvent() {

}

void GetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_get_socket_option(this);
}
