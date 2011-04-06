#include "states/SlowStart.h"


SlowStart::SlowStart() {
}

SlowStart::~SlowStart() {

}

void SlowStart::send_packet(Context* c, SendPacketEvent* e) {

}

void SlowStart::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
}

void SlowStart::send_to(Context* c, SendEvent* e) {

}

void SlowStart::state_send_buffer_not_empty(Context* c, SendBufferNotEmpty* e) {
    cout << "SlowStart::state_send_buffer_not_empty()" << endl;

    Socket* s = e->get_socket();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
    assert(cache);

    if(cache->is_empty() && s->get_send_buffer().size() > 0) {

        TCPPacket* p = new TCPPacket();

        string data = s->get_send_buffer().substr(0, p->max_data_length());
        s->get_send_buffer().erase(0, p->max_data_length());

        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        p->set_ip_protocol(SIMPLE_TCP);

        p->set_ip_destination_address_s(destination->get_address());
        p->set_destination_port(destination->get_port());

        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());

        p->set_data((unsigned char*) data.c_str(), data.size());

        Dispatcher::instance().enqueue(new SendPacketEvent(s, p));
        Dispatcher::instance().enqueue(new SendBufferNotFull(s));
    }
}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}