#include "events/Event.h"

Event::Event(Socket* s, Priority p) : socket_(s), priority_(p) {
}

Event::Event() : socket_(0), priority_(LOW) {
}

Event::~Event() {
}

Socket* Event::get_socket() {
    if (socket_ == NULL) {
        throw WiFuException("Socket not set on Event");
    } else {
        return socket_;
    }
}

void Event::set_socket(Socket* socket) {
    assert(socket);
    socket_ = socket;
}

Priority Event::get_priority() {
    return priority_;
}

void Event::set_priority(Priority priority) {
    priority_ = priority;
}
