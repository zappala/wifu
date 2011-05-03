#include "SimpleTCP.h"

SimpleTCP::SimpleTCP() : Protocol(SIMPLE_TCP) {
}

void SimpleTCP::save_socket(Socket* s, IContextContainer* icc) {
    map_[s] = icc;
}

IContextContainer* SimpleTCP::get_context(Socket* s) {
    itr_ = map_.find(s);
    assert(itr_ != map_.end());
    return itr_->second;
}

SimpleTCP& SimpleTCP::instance() {
    static SimpleTCP instance_;
    return instance_;
}

SimpleTCP::~SimpleTCP() {
}

void SimpleTCP::icontext_socket(SocketEvent* e) {
    Socket* s = e->get_socket();
    save_socket(s);
    IContextContainer* c = get_context(s);

    CacheMap::instance().put(s, new SimpleTCPCache());

    c->get_connection_manager()->icontext_socket(e);
    c->get_reliability()->icontext_socket(e);
    c->get_congestion_control()->icontext_socket(e);
}

void SimpleTCP::icontext_bind(BindEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_bind(e);
    c->get_reliability()->icontext_bind(e);
    c->get_congestion_control()->icontext_bind(e);
}

void SimpleTCP::icontext_listen(ListenEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_listen(e);
    c->get_reliability()->icontext_listen(e);
    c->get_congestion_control()->icontext_listen(e);
}

void SimpleTCP::icontext_receive_packet(NetworkReceivePacketEvent* e) {
//        cout << "SimpleTCP::receive_packet()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    CloseEvent* close_event = c->get_saved_close_event();
    CongestionControlContext* ccc = (CongestionControlContext*) c->get_congestion_control();

    TCPPacket* packet = (TCPPacket*) e->get_packet();
    TCPTimestampOption* ts = (TCPTimestampOption*) packet->get_option(TCPOPT_TIMESTAMP);
    if (ts) {
        c->set_echo_reply(ts->get_timestamp());
    }
//    cout << "SimpleTCP::receive_packet(), TS: " << ts->to_s() << endl;

    if (packet->is_tcp_fin() && !s->get_receive_buffer().empty()) {
        c->set_fin(e);
//                cout << "SimpleTCP::receive_packet(), FIN && receive buffer is not empty(), returning" << endl;
        return;
    }

    c->get_connection_manager()->icontext_receive_packet(e);
    c->get_reliability()->icontext_receive_packet(e);
    c->get_congestion_control()->icontext_receive_packet(e);

    if (close_event && s->get_send_buffer().empty() && ccc->get_num_outstanding() == 0) {
//                cout << "SimpleTCP::receive_packet(), sending out close event" << endl;
        c->get_connection_manager()->icontext_close(close_event);
        c->set_saved_close_event(0);
    }
}

void SimpleTCP::icontext_send_packet(SendPacketEvent* e) {
    //    cout << "SimpleTCP::send_packet()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    TCPPacket* p = (TCPPacket*) e->get_packet();
    
    TCPTimestampOption* option = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    assert(option);
    option->set_timestamp();
    if (c->get_echo_reply()) {
        option->set_echo_reply(c->get_echo_reply());
    }
//    cout << "SimpleTCP::send_packet(), TS: " << option->to_s() << endl;

    c->get_connection_manager()->icontext_send_packet(e);
    c->get_reliability()->icontext_send_packet(e);
    c->get_congestion_control()->icontext_send_packet(e);
    c->get_rate_limiter()->icontext_send_packet(e);
}

void SimpleTCP::icontext_connect(ConnectEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_connect(e);
    c->get_reliability()->icontext_connect(e);
    c->get_congestion_control()->icontext_connect(e);
}

void SimpleTCP::icontext_accept(AcceptEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_accept(e);
    c->get_reliability()->icontext_accept(e);
    c->get_congestion_control()->icontext_accept(e);
}

void SimpleTCP::icontext_new_connection_established(ConnectionEstablishedEvent* e) {
    //    cout << "SimpleTCP::new_connection_established()" << endl;

}

void SimpleTCP::icontext_new_conneciton_initiated(ConnectionInitiatedEvent* e) {
    //    cout << "SimpleTCP::new_conneciton_initiated()" << endl;

    // Get out pointers
    ConnectionInitiatedEvent* event = (ConnectionInitiatedEvent*) e;
    Socket* listening_socket = event->get_socket();
    Socket* new_socket = event->get_new_socket();

    // Get the listening socket's context and save it as the new socket's context
    IContextContainer* listening_cc = get_context(listening_socket);
    save_socket(new_socket, listening_cc);

    // Recreate the listening socket's context and save it as such
    IContextContainer* new_cc = new IContextContainer();
    save_socket(listening_socket, new_cc);

    // Move the Cache over
    Cache* listening_cache = CacheMap::instance().get(listening_socket);
    assert(listening_cache);
    CacheMap::instance().put(new_socket, listening_cache);
    CacheMap::instance().put(listening_socket, new SimpleTCPCache());

    // Tell the listening socket's (new) context that a new connection is occuring
    // (This is basically a hack so the new context can move back to the appropriate state.)
    new_cc->get_connection_manager()->icontext_new_conneciton_initiated(e);
    new_cc->get_reliability()->icontext_new_conneciton_initiated(e);
    new_cc->get_congestion_control()->icontext_new_conneciton_initiated(e);
}

void SimpleTCP::icontext_close(CloseEvent* e) {
    //    cout << "SimpleTCP::icontext_close()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);
    CongestionControlContext* ccc = (CongestionControlContext*) c->get_congestion_control();

    if (s->get_send_buffer().empty() && ccc->get_num_outstanding() == 0) {
        //        cout << "SimpleTCP::icontext_close(), calling connection manager" << endl;
        c->get_connection_manager()->icontext_close(e);
    } else {
        //        cout << "SimpleTCP::icontext_close(), saving close event" << endl;
        c->set_saved_close_event(e);
    }

    // TODO: return error, if necessary
    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(0));
    response->put(ERRNO, Utils::itoa(0));
    dispatch(response);
}

void SimpleTCP::icontext_timer_fired_event(TimerFiredEvent* e) {
    //    cout << "In SimpleTCP::timer_fired()\n";
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_timer_fired_event(e);
    c->get_reliability()->icontext_timer_fired_event(e);
    c->get_congestion_control()->icontext_timer_fired_event(e);
}

void SimpleTCP::icontext_resend_packet(ResendPacketEvent* e) {
    //    cout << "In SimpleTCP::resend_packet()\n";
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_resend_packet(e);
    c->get_reliability()->icontext_resend_packet(e);
    c->get_congestion_control()->icontext_resend_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::icontext_send(SendEvent* e) {
    //    cout << "SimpleTCP::send_to()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    bool connected = c->get_connection_manager()->icontext_can_send(s);
    bool room = is_room_in_send_buffer(e);

    if (connected && room) {
        //        cout << "SimpleTCP::send_to(), appending to send buffer" << endl;
        save_in_buffer_and_send_events(e);

    } else if (!connected) {
        //        cout << "SimpleTCP::send_to(), not connected!" << endl;
        ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
        response->put(RETURN_VALUE_STRING, Utils::itoa(-1));
        response->put(ERRNO, Utils::itoa(ENOTCONN));
        dispatch(response);

    } else {
        //        cout << "SimpleTCP::send_to(), saving send event" << endl;
        c->set_saved_send_event(e);
    }
}

void SimpleTCP::icontext_receive(ReceiveEvent* e) {
    //    cout << "SimpleTCP::receive_from()" << endl;

    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    if (s->get_receive_buffer().size() > 0) {
        //        cout << "SimpleTCP::receive_from(), dispatching data to front end" << endl;
        create_and_dispatch_received_data(e);
    } else {
        assert(c->get_saved_receive_event() == NULL);
        c->set_saved_receive_event(e);
    }

    c->get_connection_manager()->icontext_receive(e);
    c->get_reliability()->icontext_receive(e);
    c->get_congestion_control()->icontext_receive(e);
}

void SimpleTCP::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {
    //    cout << "SimpleTCP::icontext_receive_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    ReceiveEvent* receive_event = c->get_saved_receive_event();

    if (receive_event != NULL && s->get_receive_buffer().size() > 0) {
        create_and_dispatch_received_data(receive_event);
        c->set_saved_receive_event(NULL);
    }

    c->get_connection_manager()->icontext_receive_buffer_not_empty(e);
    c->get_reliability()->icontext_receive_buffer_not_empty(e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(e);
}

void SimpleTCP::icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e) {
    //    cout << "SimpleTCP::icontext_receive_buffer_not_full()" << endl;

    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_receive_buffer_not_full(e);
    c->get_reliability()->icontext_receive_buffer_not_full(e);
    c->get_congestion_control()->icontext_receive_buffer_not_full(e);

    if (c->get_fin() && s->get_receive_buffer().empty()) {
        //        cout << "SimpleTCP::icontext_receive_buffer_not_full(), sending out FIN again" << endl;
        dispatch(c->get_fin());
        c->set_fin(0);
    }
}

void SimpleTCP::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
    //    cout << "SimpleTCP::icontext_send_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_send_buffer_not_empty(e);
    c->get_reliability()->icontext_send_buffer_not_empty(e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(e);
}

void SimpleTCP::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
    //    cout << "SimpleTCP::icontext_send_buffer_not_full()" << endl;

    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    SendEvent* saved_send_event = c->get_saved_send_event();



    if (saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(saved_send_event);
    }



    c->get_connection_manager()->icontext_send_buffer_not_full(e);
    c->get_reliability()->icontext_send_buffer_not_full(e);
    c->get_congestion_control()->icontext_send_buffer_not_full(e);
}

bool SimpleTCP::icontext_can_send(Socket* s) {
    IContextContainer* c = get_context(s);
    return c->get_connection_manager()->icontext_can_send(s);
}

bool SimpleTCP::icontext_can_receive(Socket* s) {
    IContextContainer* c = get_context(s);
    return c->get_connection_manager()->icontext_can_receive(s);
}

void SimpleTCP::icontext_delete_socket(DeleteSocketEvent* e) {
    //    cout << "SimpleTCP::icontext_delete_socket()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_delete_socket(e);
    c->get_reliability()->icontext_delete_socket(e);
    c->get_congestion_control()->icontext_delete_socket(e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void SimpleTCP::icontext_set_socket_option(SetSocketOptionEvent* e) {
    //    cout << "SimpleTCP::icontext_set_socket_option()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_set_socket_option(e);
    c->get_reliability()->icontext_set_socket_option(e);
    c->get_congestion_control()->icontext_set_socket_option(e);
}

void SimpleTCP::icontext_get_socket_option(GetSocketOptionEvent* e) {
    //    cout << "SimpleTCP::icontext_get_socket_option()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_get_socket_option(e);
    c->get_reliability()->icontext_get_socket_option(e);
    c->get_congestion_control()->icontext_get_socket_option(e);
}

void SimpleTCP::send_network_packet(Socket* s, WiFuPacket* p) {
    //    cout << "SimpleTCP::send_network_packet()" << endl;
    //    cout << "SimpleTCP::send_network_packet(): Packet: " << endl;
    //
    //    cout << p->to_s_format() << endl;
    //    cout << p->to_s() << endl;

    NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(e);
}

bool SimpleTCP::is_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int num_bytes_to_send = e->data_length();
    return s->get_send_buffer().size() + num_bytes_to_send <= MAX_BUFFER_SIZE;
}

void SimpleTCP::save_in_buffer_and_send_events(SendEvent* e) {
    int num_bytes_to_send = e->data_length();
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    s->get_send_buffer().append(data, num_bytes_to_send);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(num_bytes_to_send));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    dispatch(new SendBufferNotEmptyEvent(s));
}

void SimpleTCP::create_and_dispatch_received_data(ReceiveEvent* e) {
    //    cout << "SimpleTCP::create_and_dispatch_received_data()" << endl;
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();

    string data = s->get_receive_buffer().substr(0, buffer_size);
    s->get_receive_buffer().erase(0, data.size());

    //    cout << "SimpleTCP::create_and_dispatch_received_data(), Buffer: \"" << data << "\"" << endl;

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    //    cout << "SimpleTCP::create_and_dispatch_received_data(), dispatching receive buffer not full event" << endl;
    dispatch(new ReceiveBufferNotFullEvent(s));
}


