#include "events/SetSocketOptionEvent.h"

SetSocketOptionEvent::SetSocketOptionEvent(string& message, string& file, Socket* s) : SocketOptionEvent(message, file, s) {

}

SetSocketOptionEvent::~SetSocketOptionEvent() {

}

void SetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_set_socket_option(this);
}

string& SetSocketOptionEvent::get_option_value() {
    return get_map()[OPTION_VALUE_STRING];
}