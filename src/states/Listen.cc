#include "states/Listen.h"

Listen::Listen() {

}

Listen::~Listen() {

}

void Listen::enter(Context* c) {

}

void Listen::exit(Context* c) {

}

void Listen::receive(Context* c, Socket* s, Packet* p) {
    cout << "Listen: Receive on socket: " << s->get_socket_id() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;
    cout << "Listen::receive(), data length: " << packet->data_length() << endl;
    cout << "Listen::receive(), is SYN: " << endl;

    assert(packet->is_tcp_syn());
    
    if(packet->is_tcp_syn()) {
        cout << "Listen: receive(), processing TCP SYN" << endl;
        

        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_destination();
        AddressPort* destination = packet->get_source();

        TCPPacket* response = new TCPPacket(source, destination, data, 0);
        response->set_tcp_syn(true);
        response->set_tcp_ack(true);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);
        cout << "Listen: Dispatcher Enqueue: " << type_name(*e) << endl;

        cmc->set_state(new SynReceived());
        return;
    }

    // Should never receive a different type of packet in the Listent State
    assert(false);
    
    
}
