#include "events/framework_events/LibraryEvent.h"

LibraryEvent::~LibraryEvent() {

}

LibraryEvent::LibraryEvent() : BufferEvent() {

}

void LibraryEvent::save_buffer(unsigned char* buffer, u_int32_t length) {
    memcpy(get_buffer(), buffer, length);
    message_ = reinterpret_cast<FrontEndMessage*>(get_buffer());
    buffer_length_ = length;
}

struct sockaddr_un* LibraryEvent::get_source() const {
    return &(message_->source);
}

u_int32_t LibraryEvent::get_message_type() const {
    return message_->message_type;
}

u_int32_t LibraryEvent::get_buffer_length() const {
    return buffer_length_;
}

int LibraryEvent::get_fd() const {
    return message_->fd;
}
