#include "states/Established.h"
#include "events/NetworkReceivePacketEvent.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
    cout << "Established::enter()" << endl;

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
        default:
            break;
    }



}

void Established::exit(Context* c) {

}

void Established::receive_packet(Context* c, Socket* s, WiFuPacket* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*)p;

//    if(packet->is_tcp_syn() && packet->is_tcp_ack()) {
//        cmc->set_state(new SynSent());
//        Event* e = new NetworkReceivePacketEvent(s, p);
//        Dispatcher::instance().enqueue(e);
//        return;
//    }
}
