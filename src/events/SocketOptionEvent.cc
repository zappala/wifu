#include "events/SocketOptionEvent.h"

SocketOptionEvent::SocketOptionEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

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