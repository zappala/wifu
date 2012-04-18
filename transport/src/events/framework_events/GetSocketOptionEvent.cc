#include "events/framework_events/GetSocketOptionEvent.h"

GetSocketOptionEvent::GetSocketOptionEvent() : SocketOptionEvent() {
    
}

GetSocketOptionEvent::~GetSocketOptionEvent() {

}

void GetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_get_socket_option(this);
}
