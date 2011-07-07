#include "events/framework_events/SetSocketOptionEvent.h"

SetSocketOptionEvent::SetSocketOptionEvent(gcstring& message, gcstring& file, Socket* s) : SocketOptionEvent(message, file, s) {

}

SetSocketOptionEvent::~SetSocketOptionEvent() {

}

void SetSocketOptionEvent::execute(IModule* m) {
    m->imodule_library_set_socket_option(this);
}

gcstring& SetSocketOptionEvent::get_option_value() {
    return get_map()[OPTION_VALUE_STRING];
}

pair<gcstring, socklen_t> SetSocketOptionEvent::get_value_length_pair() {
    return make_pair(get_option_value(), get_option_length());
}