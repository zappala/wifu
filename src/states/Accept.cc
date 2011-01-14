#include "states/Accept.h"

Accept::Accept() {

}

Accept::~Accept() {

}

void Accept::enter(Context* c) {

}

void Accept::exit(Context* c) {

}

void Accept::receive(Context* c, Socket* s, Packet* p) {
    cout << "Accept: Receive on socket: " << s->get_socket_id() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;
    cout << "Accept::receive(), data length: " << packet->data_length() << endl;
    cout << "Accept::receive(), is SYN: " << endl;

    assert(packet->is_tcp_syn());

    if(packet->is_tcp_syn()) {
        cout << "Accept: receive(), processing TCP SYN" << endl;


        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_destination();
        AddressPort* destination = packet->get_source();

        TCPPacket* response = new TCPPacket(source, destination, data, 0);
        response->set_tcp_syn(true);
        response->set_tcp_ack(true);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);
        cout << "Accept: Dispatcher Enqueue: " << type_name(*e) << endl;

        cmc->set_state(new SynReceived());
        return;
    }

    // Should never receive a different type of packet in the Listent State
    assert(false);


}
