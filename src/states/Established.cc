#include "states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
//    cout << "Established::enter()" << endl;

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    ResponseEvent* response;

    ConnectEvent* event = cmc->get_connect_event();


    switch (cmc->get_connection_type()) {
        case ACTIVE_OPEN:
            cout << "Established::enter(), Active Open" << endl;
            response = new ResponseEvent(event->get_socket(), event->get_name(), event->get_map()[FILE_STRING]);
            response->put(ERRNO, Utils::itoa(0));
            response->put(RETURN_VALUE_STRING, Utils::itoa(0));
            Dispatcher::instance().enqueue(response);
            break;

        case ESTABLISHED:
            cout << "Established::enter(), Established" << endl;
            break;
        case PASSIVE_OPEN:
            cout << "Established::enter(), Passive Open" << endl;
            break;
        default:
            break;
    }



}

void Established::exit(Context* c) {

}

void Established::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) e->get_packet();

    if (packet->is_tcp_syn()) {
        // ignore potential resends of three-way handshake packets
        return;
    }
}

void Established::receive_from(Context* c, ReceiveEvent* e) {
//    cout << "Established::receive_from()" << endl;
}

bool Established::state_can_receive(Context*, Socket* s) {
    return true;
}

bool Established::state_can_send(Context*, Socket* s) {
    return true;
}
