#include "events/Event.h"

Event::Event(int socket) : socket_(socket) {

}

Event::Event() : socket_(0) {

}

Event::~Event() {
    
}

int & Event::get_socket() {
    assert(socket_);
    return socket_;
}

void Event::set_socket(int socket) {
    assert(socket);
    socket_ = socket;
}

bool Event::less_than(Event* rhs) {
    return true;
}
