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


LibraryEvent::LibraryEvent() : FrameworkEvent() {

}

void LibraryEvent::save_buffer(unsigned char* buffer, u_int32_t length) {
    memcpy(buffer_, buffer, length);
    message_ = reinterpret_cast<FrontEndMessage*>(buffer_);
    buffer_length_ = length;
}

struct sockaddr_un* LibraryEvent::get_source() const {
    return &(message_->source);
}

u_int32_t LibraryEvent::get_message_type() const {
    return message_->message_type;
}

unsigned char* LibraryEvent::get_buffer() {
    return buffer_;
}

u_int32_t LibraryEvent::get_buffer_length() const {
    return buffer_length_;
}

void LibraryEvent::execute(IModule* m) {
    throw WiFuException("Need child class");
}