#include "events/framework_events/SocketOptionEvent.h"

SocketOptionEvent::SocketOptionEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

SocketOptionEvent::SocketOptionEvent() : LibraryEvent() {
    
}

SocketOptionEvent::~SocketOptionEvent() {

}

int SocketOptionEvent::get_option_name() {
    return ((struct SockOptMessage*) get_buffer())->optname;
}

int SocketOptionEvent::get_level() {
    return ((struct SockOptMessage*) get_buffer())->level;
}

socklen_t SocketOptionEvent::get_option_length() {
    return ((struct SockOptMessage*) get_buffer())->optlen;
}

pair<int, int> SocketOptionEvent::get_level_name_pair() {
    return make_pair(get_level(), get_option_name());
}