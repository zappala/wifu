#include "states/SimpleTCPReliability.h"

SimpleTCPReliability::SimpleTCPReliability() {
}

SimpleTCPReliability::~SimpleTCPReliability() {
}

void SimpleTCPReliability::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    //    cout << "ReliabilityState::send_packet()" << endl;

    SimpleTCPReliabilityContext* rc = (SimpleTCPReliabilityContext*) c;
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

        cache->save_packet(p);

        TimeoutEvent* timeout_event = new TimeoutEvent(s, 1, 0);

//        cout << "ReliabilityState::send_packet(), timeout event: " << timeout_event << endl;
//        cout << "Current Time: " << (new TimeoutEvent(s, 0,0))->to_s() << endl;
//        cout << "ReliabilityState::send_packet(), timeout event to go off at: " << timeout_event->to_s() << endl;
//        cout << "ReliabilityState::send_packet(), packet info for cached packet: " << p << endl;
//        cout << p->to_s_format() << endl;
//        cout << p->to_s() << endl;

        rc->set_timeout_event(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }
}

void SimpleTCPReliability::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    //    cout << "ReliabilityState::timer_fired()" << endl;
    SimpleTCPReliabilityContext* rc = (SimpleTCPReliabilityContext*) c;

    if (rc->get_timeout_event() != e->get_timeout_event()) {
        //        cout << "ReliabilityState::timer_fired(), not my timeout" << endl;
        return;
    }


    Socket* s = e->get_socket();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);

    TCPPacket* p = (TCPPacket*) cache->get_packet();

    if (p) {
//        cout << endl << "ReliabilityState::timer_fired(), timeout event: " << e->get_timeout_event() << endl;
//        cout << "ReliabilityState::timer_fired(), timeout event for time: " << e->get_timeout_event()->to_s() << endl;
////        cout << "Current Time: " << (new TimeoutEvent(s, 0,0))->to_s() << endl;
//        cout << "ReliabilityState::timer_fired(), cached packet is NOT null, packet: " << p << endl;
//        cout << p->to_s_format() << endl;
//        cout << p->to_s() << endl;

        //        cache->save_packet(0);
        ResendPacketEvent* event = new ResendPacketEvent(e->get_socket(), p);
        Dispatcher::instance().enqueue(event);

        TimeoutEvent* timeout_event = new TimeoutEvent(s, 1, 0);
        rc->set_timeout_event(timeout_event);
        Dispatcher::instance().enqueue(timeout_event);
    }
}

void SimpleTCPReliability::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
        //cout << "ReliabilityState::receive_packet()" << endl;
    SimpleTCPReliabilityContext* rc = (SimpleTCPReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();
    SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
    TCPPacket* cached_packet = (TCPPacket*) cache->get_packet();


    if (p->is_tcp_ack() && cached_packet && p->get_tcp_ack_number() - 1 == cached_packet->get_tcp_sequence_number()) {
        //        cout << "ReliabilityState::receive_packet(), Case 1" << endl;
        cache->save_packet(0);
        rc->set_timeout_event(0);
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
        p->insert_tcp_header_option(new TCPTimestampOption());

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