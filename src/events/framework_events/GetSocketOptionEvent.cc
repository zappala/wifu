#include "events/framework_events/GetSocketOptionEvent.h"

GetSocketOptionEvent::GetSocketOptionEvent(gcstring_map& m, gcstring& file, Socket* s) : SocketOptionEvent(m, file, s) {

}

GetSocketOptionEvent::GetSocketOptionEvent() : SocketOptionEvent() {
    
}

GetSocketOptionEvent::~GetSocketOptionEvent() {

}

void GetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_get_socket_option(this);
}
