#include "states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    ResponseEvent* response;

    ConnectEvent* event = cmc->get_connect_event();
    

    switch (cmc->get_connection_type()) {
        case ACTIVE_OPEN:

            event->set_socket_destination();
            
            response = new ResponseEvent(event->get_socket(), event->get_name(), event->get_map()[FILE_STRING]);
            response->put(ERRNO, Utils::itoa(0));
            response->put(RETURN_VALUE_STRING, Utils::itoa(0));
            Dispatcher::instance().enqueue(response);
            break;

        case ESTABLISHED:
            break;
        default:
            break;
    }



}

void Established::exit(Context* c) {

}

void Established::receive(Context* c, Socket* s, WiFuPacket* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}
