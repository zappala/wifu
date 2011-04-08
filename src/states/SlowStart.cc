#include "states/SlowStart.h"

SlowStart::SlowStart() {
}

SlowStart::~SlowStart() {

}

void SlowStart::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    cout << "SlowStart::receive_packet()" << endl;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
    
    if (p->get_data_length_bytes() > 0) {
        TCPPacket* p = new TCPPacket();
        p->set_ip_protocol(SIMPLE_TCP);

        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        p->set_ip_destination_address_s(destination->get_address());
        p->set_destination_port(destination->get_port());

        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());

        unsigned char* data = (unsigned char*) "";
        p->set_data(data, 0);

        Event* spe = new SendPacketEvent(s, p);
        
        cout << "SlowStart::receive_packet() A: Packet: " << p << endl;

        Dispatcher::instance().enqueue(spe);
    }
    else if(cache->is_empty() && s->get_send_buffer().size() > 0) {
        // send data
//        TCPPacket* p = new TCPPacket();
//
//        string data = s->get_send_buffer().substr(0, p->max_data_length());
//        s->get_send_buffer().erase(0, data.size());
//
//        AddressPort* destination = s->get_remote_address_port();
//        AddressPort* source = s->get_local_address_port();
//
//        p->set_ip_protocol(SIMPLE_TCP);
//
//        p->set_ip_destination_address_s(destination->get_address());
//        p->set_destination_port(destination->get_port());
//
//        p->set_ip_source_address_s(source->get_address());
//        p->set_source_port(source->get_port());
//
//        p->set_data((unsigned char*) data.c_str(), data.size());
//
//        Event* spe = new SendPacketEvent(s, p);

//        cout << "SlowStart::receive_packet() B: Packet: " << p << endl;

//        Dispatcher::instance().enqueue(spe);
//        Dispatcher::instance().enqueue(new SendBufferNotFullEvent(s));
    }
    
}

void SlowStart::state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e) {
    cout << "SlowStart::state_send_buffer_not_empty()" << endl;

    Socket* s = e->get_socket();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
    assert(cache);

    if (cache->is_empty() && s->get_send_buffer().size() > 0) {

        TCPPacket* p = new TCPPacket();

        string data = s->get_send_buffer().substr(0, p->max_data_length());
        s->get_send_buffer().erase(0, data.size());

        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        p->set_ip_protocol(SIMPLE_TCP);

        p->set_ip_destination_address_s(destination->get_address());
        p->set_destination_port(destination->get_port());

        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());

        p->set_data((unsigned char*) data.c_str(), data.size());

        Event* spe = new SendPacketEvent(s, p);
        Event* sbnf = new SendBufferNotFullEvent(s);

        cout << "SlowStart::state_send_buffer_not_empty(): Packet: " << p << endl;

        Dispatcher::instance().enqueue(spe);
        Dispatcher::instance().enqueue(sbnf);
    }
}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}