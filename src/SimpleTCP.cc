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

void SimpleTCP::socket(SocketEvent* e) {
    Socket* s = e->get_socket();
    save_socket(s);
    IContextContainer* c = get_context(s);

    CacheMap::instance().put(s, new SimpleTCPCache());

    c->get_connection_manager()->socket(e);
    c->get_reliability()->socket(e);
    c->get_congestion_control()->socket(e);
}

void SimpleTCP::bind(BindEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->bind(e);
    c->get_reliability()->bind(e);
    c->get_congestion_control()->bind(e);
}

void SimpleTCP::listen(ListenEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->listen(e);
    c->get_reliability()->listen(e);
    c->get_congestion_control()->listen(e);
}

void SimpleTCP::receive_packet(NetworkReceivePacketEvent* e) {
    cout << "SimpleTCP::receive_packet()" << endl;
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->receive_packet(e);
    c->get_reliability()->receive_packet(e);
    c->get_congestion_control()->receive_packet(e);
}

void SimpleTCP::send_packet(SendPacketEvent* e) {
    cout << "SimpleTCP::send_packet()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->send_packet(e);
    c->get_reliability()->send_packet(e);
    c->get_congestion_control()->send_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::connect(ConnectEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->connect(e);
    c->get_reliability()->connect(e);
    c->get_congestion_control()->connect(e);
}

void SimpleTCP::accept(AcceptEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->accept(e);
    c->get_reliability()->accept(e);
    c->get_congestion_control()->accept(e);
}

void SimpleTCP::new_connection_established(ConnectionEstablishedEvent* e) {
    cout << "SimpleTCP::new_connection_established()" << endl;

}

void SimpleTCP::new_conneciton_initiated(ConnectionInitiatedEvent* e) {
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
    new_cc->get_connection_manager()->new_conneciton_initiated(e);
    new_cc->get_reliability()->new_conneciton_initiated(e);
    new_cc->get_congestion_control()->new_conneciton_initiated(e);
}

void SimpleTCP::close() {
}

void SimpleTCP::timer_fired_event(TimerFiredEvent* e) {
        cout << "In SimpleTCP::timer_fired()\n";
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->timer_fired_event(e);
    c->get_reliability()->timer_fired_event(e);
    c->get_congestion_control()->timer_fired_event(e);
}

void SimpleTCP::resend_packet(ResendPacketEvent* e) {
    cout << "In SimpleTCP::resend_packet()\n";
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->resend_packet(e);
    c->get_reliability()->resend_packet(e);
    c->get_congestion_control()->resend_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::send_to(SendEvent* e) {
    cout << "SimpleTCP::send_to()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    bool connected = c->get_connection_manager()->is_connected(s);
    bool room = is_room_in_send_buffer(e);

    if (connected && room) {
        cout << "SimpleTCP::send_to(), appending to send buffer" << endl;
        save_in_buffer_and_send_events(e);

    } else if (!connected) {
        cout << "SimpleTCP::send_to(), not connected!" << endl;
        ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
        response->put(RETURN_VALUE_STRING, Utils::itoa(-1));
        response->put(ERRNO, Utils::itoa(ENOTCONN));
        dispatch(response);

    } else {
        cout << "SimpleTCP::send_to(), saving send event" << endl;
        c->set_saved_send_event(e);
    }
}

void SimpleTCP::receive_from(ReceiveEvent* e) {
    cout << "SimpleTCP::receive_from()" << endl;

    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    if(s->get_receive_buffer().size() > 0) {
        create_and_dispatch_received_data(e);
    }
    else {
        assert(c->get_saved_receive_event() == NULL);
        c->set_saved_receive_event(e);
    }
    
    c->get_connection_manager()->receive_from(e);
    c->get_reliability()->receive_from(e);
    c->get_congestion_control()->receive_from(e);
}

void SimpleTCP::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    ReceiveEvent* receive_event = c->get_saved_receive_event();

    if(receive_event != NULL && s->get_receive_buffer().size() > 0) {
        create_and_dispatch_received_data(receive_event);
        c->set_saved_receive_event(NULL);
    }

    c->get_connection_manager()->icontext_receive_buffer_not_empty(e);
    c->get_reliability()->icontext_receive_buffer_not_empty(e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(e);
}

void SimpleTCP::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
    cout << "SimpleTCP::icontext_send_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->icontext_send_buffer_not_empty(e);
    c->get_reliability()->icontext_send_buffer_not_empty(e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(e);
}

void SimpleTCP::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
    cout << "SimpleTCP::icontext_send_buffer_not_full()" << endl;

    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    SendEvent* saved_send_event = c->get_saved_send_event();

    if(saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(saved_send_event);
    }

    c->get_connection_manager()->icontext_send_buffer_not_full(e);
    c->get_reliability()->icontext_send_buffer_not_full(e);
    c->get_congestion_control()->icontext_send_buffer_not_full(e);
}

bool SimpleTCP::is_connected(Socket* s) {
    IContextContainer* c = get_context(s);
    return c->get_connection_manager()->is_connected(s);
}

void SimpleTCP::send_network_packet(Socket* s, WiFuPacket* p) {
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
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();
    
    string data = s->get_receive_buffer().substr(0, buffer_size);
    s->get_receive_buffer().erase(0, data.size());

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);

}
