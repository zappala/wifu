#include "../headers/states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
    cout << "Entering Established State" << endl;

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    ResponseEvent* response;

    ConnectEvent* event = cmc->get_connect_event();
    

    switch (cmc->get_connection_type()) {
        case ACTIVE_OPEN:

            event->set_socket_destination();
            
            cout << "Established::enter(), active connection" << endl;
            response = new ResponseEvent(event->get_socket(), event->get_name(), event->get_map()[FILE_STRING]);
            response->put(ERRNO, Utils::itoa(0));
            response->put(RETURN_VALUE_STRING, Utils::itoa(0));
            Dispatcher::instance().enqueue(response);

            cout << "Local AddressPort: " << event->get_socket()->get_local_address_port()->to_s() << endl;
            cout << "Remote AddressPort: " << event->get_socket()->get_remote_address_port()->to_s() << endl;

            break;

        

        case ESTABLISHED:
            break;
        default:
            break;
    }



}

void Established::exit(Context* c) {

}

void Established::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}
