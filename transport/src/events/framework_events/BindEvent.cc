#include "events/framework_events/BindEvent.h"

BindEvent::BindEvent() : LibraryEvent() {

}

BindEvent::~BindEvent() {

}

void BindEvent::execute(IModule* m) {
    m->imodule_library_bind(this);
}

struct sockaddr_in* BindEvent::get_addr() {
    return &(((struct BindMessage*) get_buffer())->addr);
}

socklen_t BindEvent::get_addr_len() {
    return ((struct BindMessage*) get_buffer())->len;
}
