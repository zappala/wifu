#include "events/framework_events/SocketOptionEvent.h"

SocketOptionEvent::SocketOptionEvent(gcstring_map& m, gcstring& file, Socket* s) : LibraryEvent(m, file, s) {

}

SocketOptionEvent::SocketOptionEvent() : LibraryEvent() {
    
}

SocketOptionEvent::~SocketOptionEvent() {

}

int SocketOptionEvent::get_option_name() {
    return atoi(get_map()[OPTION_NAME_STRING].c_str());
}

int SocketOptionEvent::get_level() {
    return atoi(get_map()[LEVEL_STRING].c_str());
}

socklen_t SocketOptionEvent::get_option_length() {
    return atoi(get_map()[LENGTH_STRING].c_str());
}

pair<int, int> SocketOptionEvent::get_level_name_pair() {
    return make_pair(get_level(), get_option_name());
}