#include "Event.h"

Event::Event(int socket) : Identifiable(), socket_(socket) {

}

Event::~Event() {
    
}

int & Event::get_socket() {
    return socket_;
}
