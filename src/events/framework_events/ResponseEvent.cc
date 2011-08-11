#include "events/framework_events/ResponseEvent.h"

ResponseEvent::ResponseEvent() : BufferEvent() {
    response_ = (struct GenericResponseMessage*) get_buffer();
}

ResponseEvent::~ResponseEvent() {

}

void ResponseEvent::set_message_type(u_int32_t message_type) {
    response_->message_type = message_type;
}

void ResponseEvent::set_default_length() {
    response_->length = sizeof(struct GenericResponseMessage);
}

void ResponseEvent::set_length(u_int32_t length) {
    response_->length = length;
}

u_int32_t ResponseEvent::get_length() const {
    return response_->length;
}

void ResponseEvent::set_fd(int fd) {
    response_->fd = fd;
}

void ResponseEvent::set_return_value(int return_value) {
    response_->return_value = return_value;
}

void ResponseEvent::set_errno(int error) {
    response_->error = error;
}

struct sockaddr_un* ResponseEvent::get_destination() const {
    return destination_;
}

void ResponseEvent::set_destination(struct sockaddr_un* destination) {
    memcpy(destination_, destination, sizeof (struct sockaddr_un));
}

struct GenericResponseMessage* ResponseEvent::get_response() {
    return response_;
}

void ResponseEvent::execute(IModule* m) {
    m->imodule_library_response(this);
}
