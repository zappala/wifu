#include "events/framework_events/LibraryEvent.h"

LibraryEvent::LibraryEvent(gcstring_map& m, gcstring& file, Socket* socket) : FrameworkEvent(socket), file_(file) {
    m_ = m;
    name_ = m_[NAME_STRING];
    m_[SOCKET_STRING] = Utils::itoa(socket->get_socket_id());
}

LibraryEvent::~LibraryEvent() {
    
}

gcstring& LibraryEvent::get_file() {
    return file_;
}

gcstring& LibraryEvent::get_name() {
    return name_;
}

gcstring_map& LibraryEvent::get_map() {
    return m_;
}
