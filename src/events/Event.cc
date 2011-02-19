#include "events/Event.h"

Event::Event(Socket* s) : socket_(s) {}

Event::Event() : socket_(0) {}

Event::~Event() {}

Socket* Event::get_socket() {
    assert(socket_);
    return socket_;
}

void Event::set_socket(Socket* socket) {
    assert(socket);
    socket_ = socket;
}

bool Event::less_than(Event* rhs) {
    return true;
}
