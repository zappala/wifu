#include "protocol/SimpleTCP.h"

SimpleTCP::SimpleTCP() : Protocol(SIMPLE_TCP) {
}

void SimpleTCP::save_socket(Socket* s, SimpleTCPIContextContainer* icc) {
    map_[s] = icc;
}

SimpleTCPIContextContainer* SimpleTCP::get_context(Socket* s) {
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

void SimpleTCP::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    Socket* s = e->get_socket();
    save_socket(s);
    SimpleTCPIContextContainer* c = get_context(s);

    CacheMap::instance().put(s, new SimpleTCPCache());

    c->get_connection_manager()->icontext_socket(q, e);
    c->get_reliability()->icontext_socket(q, e);
    c->get_congestion_control()->icontext_socket(q, e);
}

void SimpleTCP::icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) {
    SimpleTCPIContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_bind(q, e);
    c->get_reliability()->icontext_bind(q, e);
    c->get_congestion_control()->icontext_bind(q, e);
}

void SimpleTCP::icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) {
    SimpleTCPIContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_listen(q, e);
    c->get_reliability()->icontext_listen(q, e);
    c->get_congestion_control()->icontext_listen(q, e);
}

void SimpleTCP::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    //        cout << "SimpleTCP::receive_packet()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    CloseEvent* close_event = c->get_saved_close_event();
    SimpleTCPCongestionControlContext* ccc = (SimpleTCPCongestionControlContext*) c->get_congestion_control();

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

    c->get_connection_manager()->icontext_receive_packet(q, e);
    c->get_reliability()->icontext_receive_packet(q, e);
    c->get_congestion_control()->icontext_receive_packet(q, e);

    if (close_event && s->get_send_buffer().empty() && ccc->get_num_outstanding() == 0) {
        //                cout << "SimpleTCP::receive_packet(), sending out close event" << endl;
        c->get_connection_manager()->icontext_close(q, close_event);
        c->set_saved_close_event(0);
    }
}

void SimpleTCP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    //    cout << "SimpleTCP::send_packet()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    TCPPacket* p = (TCPPacket*) e->get_packet();

    TCPTimestampOption* option = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    assert(option);
    option->set_timestamp();
    if (c->get_echo_reply()) {
        option->set_echo_reply(c->get_echo_reply());
    }
    //    cout << "SimpleTCP::send_packet(), TS: " << option->to_s() << endl;

    c->get_connection_manager()->icontext_send_packet(q, e);
    c->get_reliability()->icontext_send_packet(q, e);
    c->get_congestion_control()->icontext_send_packet(q, e);
    c->get_rate_limiter()->icontext_send_packet(q, e);
}

void SimpleTCP::icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) {
    SimpleTCPIContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_connect(q, e);
    c->get_reliability()->icontext_connect(q, e);
    c->get_congestion_control()->icontext_connect(q, e);
}

void SimpleTCP::icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) {
    SimpleTCPIContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_accept(q, e);
    c->get_reliability()->icontext_accept(q, e);
    c->get_congestion_control()->icontext_accept(q, e);
}

void SimpleTCP::icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {
    //    cout << "SimpleTCP::new_connection_established()" << endl;

}

void SimpleTCP::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
    //    cout << "SimpleTCP::new_conneciton_initiated()" << endl;

    // Get out pointers
    ConnectionInitiatedEvent* event = (ConnectionInitiatedEvent*) e;
    Socket* listening_socket = event->get_socket();
    Socket* new_socket = event->get_new_socket();

    // Get the listening socket's context and save it as the new socket's context
    SimpleTCPIContextContainer* listening_cc = get_context(listening_socket);
    save_socket(new_socket, listening_cc);

    // Recreate the listening socket's context and save it as such
    SimpleTCPIContextContainer* new_cc = new SimpleTCPIContextContainer();
    save_socket(listening_socket, new_cc);

    // Move the Cache over
    Cache* listening_cache = CacheMap::instance().get(listening_socket);
    assert(listening_cache);
    CacheMap::instance().put(new_socket, listening_cache);
    CacheMap::instance().put(listening_socket, new SimpleTCPCache());

    // Tell the listening socket's (new) context that a new connection is occuring
    // (This is basically a hack so the new context can move back to the appropriate state.)
    new_cc->get_connection_manager()->icontext_new_connection_initiated(q, e);
    new_cc->get_reliability()->icontext_new_connection_initiated(q, e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(q, e);
}

void SimpleTCP::icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) {
    //    cout << "SimpleTCP::icontext_close()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);
    SimpleTCPCongestionControlContext* ccc = (SimpleTCPCongestionControlContext*) c->get_congestion_control();

    if (s->get_send_buffer().empty() && ccc->get_num_outstanding() == 0) {
        //        cout << "SimpleTCP::icontext_close(), calling connection manager" << endl;
        c->get_connection_manager()->icontext_close(q, e);
    } else {
        //        cout << "SimpleTCP::icontext_close(), saving close event" << endl;
        c->set_saved_close_event(e);
    }

    // TODO: return error, if necessary
    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(s);
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(0);
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());
    dispatch(response_event);
}

void SimpleTCP::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    //    cout << "In SimpleTCP::timer_fired()\n";
    SimpleTCPIContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->icontext_timer_fired_event(q, e);
    c->get_reliability()->icontext_timer_fired_event(q, e);
    c->get_congestion_control()->icontext_timer_fired_event(q, e);
    c->get_rate_limiter()->icontext_timer_fired_event(q, e);
}

void SimpleTCP::icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    //    cout << "In SimpleTCP::resend_packet()\n";
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    e->get_packet()->set_ip_protocol(s->get_protocol());

    c->get_connection_manager()->icontext_resend_packet(q, e);
    c->get_reliability()->icontext_resend_packet(q, e);
    c->get_congestion_control()->icontext_resend_packet(q, e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {
    //    cout << "SimpleTCP::send_to()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    bool connected = c->get_connection_manager()->icontext_can_send(s);
    bool room = is_room_in_send_buffer(e);

    if (connected && room) {
        //        cout << "SimpleTCP::send_to(), appending to send buffer" << endl;
        save_in_buffer_and_send_events(e);

    } else if (!connected) {
        //        cout << "SimpleTCP::send_to(), not connected!" << endl;

        ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
        response_event->set_socket(s);
        response_event->set_message_type(e->get_message_type());
        response_event->set_fd(e->get_fd());
        response_event->set_return_value(-1);
        response_event->set_errno(ENOTCONN);
        response_event->set_default_length();
        response_event->set_destination(e->get_source());
        dispatch(response_event);

    } else {
        //        cout << "SimpleTCP::send_to(), saving send event" << endl;
        c->set_saved_send_event(e);
    }
}

void SimpleTCP::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    //    cout << "SimpleTCP::receive_from()" << endl;

    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    if (s->get_receive_buffer().size() > 0) {
        //        cout << "SimpleTCP::receive_from(), dispatching data to front end" << endl;
        create_and_dispatch_received_data(e);
    } else {
        assert(c->get_saved_receive_event() == NULL);
        c->set_saved_receive_event(e);
    }

    c->get_connection_manager()->icontext_receive(q, e);
    c->get_reliability()->icontext_receive(q, e);
    c->get_congestion_control()->icontext_receive(q, e);
}

void SimpleTCP::icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    //    cout << "SimpleTCP::icontext_receive_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    ReceiveEvent* receive_event = c->get_saved_receive_event();

    if (receive_event != NULL && s->get_receive_buffer().size() > 0) {
        create_and_dispatch_received_data(receive_event);
        c->set_saved_receive_event(NULL);
    }

    c->get_connection_manager()->icontext_receive_buffer_not_empty(q, e);
    c->get_reliability()->icontext_receive_buffer_not_empty(q, e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(q, e);
}

void SimpleTCP::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
    //    cout << "SimpleTCP::icontext_receive_buffer_not_full()" << endl;

    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_receive_buffer_not_full(q, e);
    c->get_reliability()->icontext_receive_buffer_not_full(q, e);
    c->get_congestion_control()->icontext_receive_buffer_not_full(q, e);

    if (c->get_fin() && s->get_receive_buffer().empty()) {
        //        cout << "SimpleTCP::icontext_receive_buffer_not_full(), sending out FIN again" << endl;
        dispatch(c->get_fin());
        c->set_fin(0);
    }
}

void SimpleTCP::icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    //    cout << "SimpleTCP::icontext_send_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_send_buffer_not_empty(q, e);
    c->get_reliability()->icontext_send_buffer_not_empty(q, e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(q, e);
}

void SimpleTCP::icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {
    //    cout << "SimpleTCP::icontext_send_buffer_not_full()" << endl;

    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    SendEvent* saved_send_event = c->get_saved_send_event();



    if (saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(saved_send_event);
    }



    c->get_connection_manager()->icontext_send_buffer_not_full(q, e);
    c->get_reliability()->icontext_send_buffer_not_full(q, e);
    c->get_congestion_control()->icontext_send_buffer_not_full(q, e);
}

bool SimpleTCP::icontext_can_send(Socket* s) {
    SimpleTCPIContextContainer* c = get_context(s);
    return c->get_connection_manager()->icontext_can_send(s);
}

bool SimpleTCP::icontext_can_receive(Socket* s) {
    SimpleTCPIContextContainer* c = get_context(s);
    return c->get_connection_manager()->icontext_can_receive(s);
}

void SimpleTCP::icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) {
    //    cout << "SimpleTCP::icontext_delete_socket()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_delete_socket(q, e);
    c->get_reliability()->icontext_delete_socket(q, e);
    c->get_congestion_control()->icontext_delete_socket(q, e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void SimpleTCP::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    //    cout << "SimpleTCP::icontext_set_socket_option()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_set_socket_option(q, e);
    c->get_reliability()->icontext_set_socket_option(q, e);
    c->get_congestion_control()->icontext_set_socket_option(q, e);
}

void SimpleTCP::icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {
    //    cout << "SimpleTCP::icontext_get_socket_option()" << endl;
    Socket* s = e->get_socket();
    SimpleTCPIContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_get_socket_option(q, e);
    c->get_reliability()->icontext_get_socket_option(q, e);
    c->get_congestion_control()->icontext_get_socket_option(q, e);
}

bool SimpleTCP::is_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int num_bytes_to_send = e->get_data_length();
    return s->get_send_buffer().size() + num_bytes_to_send <= UNIX_SOCKET_MAX_BUFFER_SIZE;
}

void SimpleTCP::save_in_buffer_and_send_events(SendEvent* e) {
    int num_bytes_to_send = e->get_data_length();
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    s->get_send_buffer().append(data, num_bytes_to_send);

    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(s);
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(num_bytes_to_send);
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());

    dispatch(response_event);
    dispatch(new SendBufferNotEmptyEvent(s));
}

void SimpleTCP::create_and_dispatch_received_data(ReceiveEvent* e) {
    //    cout << "SimpleTCP::create_and_dispatch_received_data()" << endl;
    Socket* s = e->get_socket();
    size_t buffer_size = e->get_data_length();

    RecvFromResponseEvent* response = (RecvFromResponseEvent*) ObjectPool<ResponseEvent>::instance().get();
    response->set_socket(s);
    response->set_message_type(e->get_message_type());
    response->set_fd(e->get_fd());
    // done in set_return_buffer()
    // response->set_return_value(length);
    response->set_errno(0);
    // done in set_return_buffer()
    // response->set_return_value(length);
    response->set_destination(e->get_source());
    response->set_addr(s->get_remote_address_port()->get_network_struct_ptr(), sizeof (struct sockaddr_in));

    int length = min(s->get_receive_buffer().size(), buffer_size);
    response->set_return_buffer((unsigned char*) s->get_receive_buffer().data(), length);
    s->get_receive_buffer().erase(0, length);

    dispatch(response);
    dispatch(new ReceiveBufferNotFullEvent(s));
}


