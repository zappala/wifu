#include "states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::state_enter(Context* c) {
    // TODO: spawn new Socket.
    //    cout << "Established::enter()" << endl;

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    ConnectEvent* event = cmc->get_connect_event();


    switch (cmc->get_connection_type()) {
        case ACTIVE_OPEN:
        {
            //            cout << "Established::enter(), Active Open" << endl;
            ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
            response_event->set_default_length();
            response_event->set_destination(event->get_source());
            response_event->set_errno(0);
            response_event->set_message_type(event->get_message_type());
            response_event->set_return_value(0);
            response_event->set_socket(event->get_socket());
            response_event->set_fd(event->get_fd());

//            response = new ResponseEvent(event->get_socket(), event->get_name(), event->get_map()[FILE_STRING]);
//            response->put(ERRNO, Utils::itoa(0));
//            response->put(RETURN_VALUE_STRING, Utils::itoa(0));
            Dispatcher::instance().enqueue(response_event);
            break;
        }
        case ESTABLISHED:
        case PASSIVE_OPEN:
        default:
            break;
    }



}

void Established::state_exit(Context* c) {
    //    cout << "Established::exit()" << endl;

}

void Established::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//        cout << "Established::receive_packet()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();


    if (packet->is_tcp_syn()) {
        // TODO: this may be deleted after testing it without
        // ignore potential resends of three-way handshake packets
        return;
    }

    if (packet->is_tcp_fin()) {
        //        cout << "Established::receive_packet(), FIN" << endl;

        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        TCPPacket* response = new TCPPacket();
        response->insert_tcp_header_option(new TCPTimestampOption());
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());

        response->set_data(data, 0);

        SendPacketEvent* event = new SendPacketEvent(s, response);
        q->enqueue(event);

        cmc->set_state(new CloseWait());

        ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
        response_event->set_default_length();
        response_event->set_destination(cmc->get_front_end_socket());
        response_event->set_errno(0);
        // TODO: may need to change this to WIFU_RECVFROM
        response_event->set_message_type(WIFU_PRECLOSE);
        response_event->set_return_value(0);
        response_event->set_socket(s);
        response_event->set_fd(s->get_socket_id());
//        cout << "Established::state_receive_packet()" << endl;

        //        gcstring name = WIFU_PRECLOSE_NAME;
        //        ResponseEvent* response_event = new ResponseEvent(s, name, cmc->get_file());
        //        response_event->put(RETURN_VALUE_STRING, Utils::itoa(0));
        //        response_event->put(ERRNO, Utils::itoa(0));
        Dispatcher::instance().enqueue(response_event);

        return;
    }
}

bool Established::state_can_receive(Context*, Socket* s) {
    return true;
}

bool Established::state_can_send(Context*, Socket* s) {
    return true;
}

void Established::state_close(Context* c, QueueProcessor<Event*>* q, CloseEvent* e) {
    //    cout << "Established::state_close()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    Socket* s = e->get_socket();

    if (s->get_send_buffer().empty()) {
        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        TCPPacket* response = new TCPPacket();
        response->insert_tcp_header_option(new TCPTimestampOption());
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());

        response->set_data(data, 0);
        response->set_tcp_fin(true);

        cmc->set_state(new FinWait1());

        SendPacketEvent* event = new SendPacketEvent(s, response);
        q->enqueue(event);




        return;
    }
}