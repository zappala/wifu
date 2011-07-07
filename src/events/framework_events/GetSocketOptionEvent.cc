#include "events/framework_events/GetSocketOptionEvent.h"

GetSocketOptionEvent::GetSocketOptionEvent(gcstring& message, gcstring& file, Socket* s) : SocketOptionEvent(message, file, s) {

}

GetSocketOptionEvent::~GetSocketOptionEvent() {

}

void GetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_get_socket_option(this);
}
