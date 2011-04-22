#include "states/ReliabilityState.h"

ReliabilityState::ReliabilityState() {
}

ReliabilityState::~ReliabilityState() {
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}

void ReliabilityState::send_packet(Context* c, SendPacketEvent* e) {
//    cout << "ReliabilityState::send_packet()" << endl;

    ReliabilityContext* rc = (ReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    u_int32_t seq_num = rc->get_seq_number();
    p->set_tcp_sequence_number(seq_num);
    rc->set_seq_number(seq_num + 1);

    u_int32_t ack_num = rc->get_ack_number();
    if (ack_num != 0) {
        p->set_tcp_ack(true);
        p->set_tcp_ack_number(ack_num);
    }

    if (!p->is_naked_ack()) {
//        cout << "ReliabilityState::send_packet(), Caching the Packet: " << p << endl;

        SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
        if (cache->get_packet()) {
//            cout << "ReliabilityState::send_packet(), Packet in cache: " << cache->get_packet() << endl;
        }
//        assert(cache->get_packet() == NULL);
        cache->save_packet(p);

        TimeoutEvent* timeout_event = new TimeoutEvent(s, 1, 0);
        rc->set_timeout_event(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }
}

void ReliabilityState::timer_fired(Context* c, TimerFiredEvent* e) {
//    cout << "ReliabilityState::timer_fired()" << endl;
    ReliabilityContext* rc = (ReliabilityContext*) c;
    Socket* s = e->get_socket();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);

    WiFuPacket* p = cache->get_packet();

    if (p) {
//        cout << "ReliabilityState::timer_fired(), cached packet is NOT null, socket: " << s << endl;
//        cache->save_packet(0);
        ResendPacketEvent* event = new ResendPacketEvent(e->get_socket(), p);
        Dispatcher::instance().enqueue(event);

        TimeoutEvent* timeout_event = new TimeoutEvent(s, 1, 0);
        rc->set_timeout_event(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }
}

void ReliabilityState::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
//    cout << "ReliabilityState::receive_packet()" << endl;
    ReliabilityContext* rc = (ReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
    TCPPacket* cached_packet = (TCPPacket*) cache->get_packet();


    if (p->is_tcp_ack() && cached_packet && p->get_tcp_ack_number() - 1 == cached_packet->get_tcp_sequence_number()) {
//        cout << "ReliabilityState::receive_packet(), Case 1" << endl;
        cache->save_packet(0);
        Event* cancel_timer_event = new CancelTimerEvent(rc->get_timeout_event());
        Dispatcher::instance().enqueue(cancel_timer_event);
    }

    if (p->get_tcp_sequence_number() == rc->get_ack_number() || rc->get_ack_number() == 0) {
//        cout << "ReliabilityState::receive_packet(), Case 2" << endl;
        rc->set_ack_number(p->get_tcp_sequence_number() + 1);
        if (p->get_data_length_bytes() > 0) {
            s->get_receive_buffer().append((const char*) p->get_data(), p->get_data_length_bytes());
            Dispatcher::instance().enqueue(new ReceiveBufferNotEmptyEvent(s));
        }
    } else if (p->get_tcp_sequence_number() - 1 == rc->get_ack_number()) {
//        cout << "ReliabilityState::receive_packet(), Case 3" << endl;
        rc->set_ack_number(p->get_tcp_sequence_number() + 1);
        if (p->get_data_length_bytes() > 0) {
            s->get_receive_buffer().append((const char*) p->get_data(), p->get_data_length_bytes());
            Dispatcher::instance().enqueue(new ReceiveBufferNotEmptyEvent(s));
        }
    } else if (p->get_tcp_sequence_number() + 1 == rc->get_ack_number() && cache->is_empty()) {
//        cout << "ReliabilityState::receive_packet(), Case 4" << endl;
        rc->set_seq_number(rc->get_seq_number() - 1);

        if (p->get_data_length_bytes() > 0) {
            // will send ack in congestion control
            return;
        }

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

//        cout << "ReliabilityState::receive_packet(), Case 4, Packet: " << p << endl;

        Dispatcher::instance().enqueue(spe);
    }
//    cout << "ReliabilityState::receive_packet(), returning" << endl;
}