#include "events/Event.h"

Event::Event(int socket) : socket_(socket) {

}

Event::~Event() {
    
}

int & Event::get_socket() {
    return socket_;
}

bool Event::less_than(Event* rhs) {
    return true;
}
