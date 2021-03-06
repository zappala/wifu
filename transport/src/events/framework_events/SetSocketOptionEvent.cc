#include "events/framework_events/SetSocketOptionEvent.h"

SetSocketOptionEvent::SetSocketOptionEvent() : SocketOptionEvent() {
    
}

SetSocketOptionEvent::~SetSocketOptionEvent() {

}

void SetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_set_socket_option(this);
}

void* SetSocketOptionEvent::get_option_value() {
    return get_buffer() + sizeof(struct SetSockOptMessage);
}

pair<gcstring, socklen_t> SetSocketOptionEvent::get_value_length_pair() {
    return make_pair(gcstring((const char*) get_option_value(), get_option_length()), get_option_length());
}