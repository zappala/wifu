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

    c->get_congestion_control()->socket(e);
    c->get_connection_manager()->socket(e);
    c->get_reliability()->socket(e);
}

void SimpleTCP::bind(BindEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_congestion_control()->bind(e);
    c->get_connection_manager()->bind(e);
    c->get_reliability()->bind(e);
}

void SimpleTCP::listen(ListenEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    //    cout << "SimpleTCP::listen: Local AP: " << e->get_socket()->get_local_address_port()->to_s() << endl;
    //    cout << "SimpleTCP::listen: Remote AP: " << e->get_socket()->get_remote_address_port()->to_s() << endl;

    c->get_congestion_control()->listen(e);
    c->get_connection_manager()->listen(e);
    c->get_reliability()->listen(e);
}

void SimpleTCP::receive_packet(NetworkReceivePacketEvent* e) {
    cout << "SimpleTCP::receive_packet()" << endl;
    IContextContainer* c = get_context(e->get_socket());

    c->get_connection_manager()->receive_packet(e);
    c->get_congestion_control()->receive_packet(e);
    c->get_reliability()->receive_packet(e);
}

void SimpleTCP::send_packet(SendPacketEvent* e) {
    cout << "SimpleTCP::send_packet()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_connection_manager()->send_packet(e);
    c->get_congestion_control()->send_packet(e);
    c->get_reliability()->send_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::connect(ConnectEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_congestion_control()->connect(e);
    c->get_connection_manager()->connect(e);
    c->get_reliability()->connect(e);
}

void SimpleTCP::accept(AcceptEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_congestion_control()->accept(e);
    c->get_connection_manager()->accept(e);
    c->get_reliability()->accept(e);
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
    new_cc->get_congestion_control()->new_conneciton_initiated(e);
    new_cc->get_connection_manager()->new_conneciton_initiated(e);
    new_cc->get_reliability()->new_conneciton_initiated(e);
}

void SimpleTCP::close() {
}

void SimpleTCP::timer_fired_event(TimerFiredEvent* e) {
    //    cout << "In SimpleTCP::timer_fired()\n";
    IContextContainer* c = get_context(e->get_socket());

    c->get_congestion_control()->timer_fired_event(e);
    c->get_connection_manager()->timer_fired_event(e);
    c->get_reliability()->timer_fired_event(e);
}

void SimpleTCP::resend_packet(ResendPacketEvent* e) {
    cout << "In SimpleTCP::resend_event()\n";
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    // TODO: Does congestion control need to know about a resend?
    c->get_congestion_control()->resend_packet(e);

    // Probably don't want to change states when we resend
    // TODO: Does connection manager need to know about a resend?
    c->get_connection_manager()->resend_packet(e);

    // TODO: I think reliability needs to know about a resend (so it can start up another timer)
    c->get_reliability()->resend_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::send_to(SendEvent* e) {
    cout << "SimpleTCP::send_to()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    int num_bytes_to_send = e->data_length();

    bool connected = c->get_connection_manager()->is_connected(s);
    bool room = s->get_send_buffer().size() + num_bytes_to_send <= MAX_BUFFER_SIZE;

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);

    if (connected && room) {
        cout << "SimpleTCP::send_to(), appending to send buffer" << endl;
        const char* data = (const char*) e->get_data();
        s->get_send_buffer().append(data, num_bytes_to_send);

        response->put(RETURN_VALUE_STRING, Utils::itoa(num_bytes_to_send));
        response->put(ERRNO, Utils::itoa(0));

        dispatch(response);
        dispatch(new SendBufferNotEmpty(s));

    } else if (!connected) {
        cout << "SimpleTCP::send_to(), not connected!" << endl;
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

    c->get_congestion_control()->receive_from(e);
    c->get_connection_manager()->receive_from(e);
    c->get_reliability()->receive_from(e);
}

void SimpleTCP::icontext_receive_buffer_not_empty(ReceiveBufferNotEmpty* e) {
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->icontext_receive_buffer_not_empty(e);
    c->get_connection_manager()->icontext_receive_buffer_not_empty(e);
    c->get_reliability()->icontext_receive_buffer_not_empty(e);
}

void SimpleTCP::icontext_send_buffer_not_empty(SendBufferNotEmpty* e) {
    cout << "SimpleTCP::icontext_send_buffer_not_empty()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->icontext_send_buffer_not_empty(e);

    // TODO: likely don't need to call these two methods
    c->get_connection_manager()->icontext_send_buffer_not_empty(e);
    c->get_reliability()->icontext_send_buffer_not_empty(e);
}

void SimpleTCP::icontext_send_buffer_not_full(SendBufferNotFull* e) {
    cout << "SimpleTCP::icontext_send_buffer_not_full()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->icontext_send_buffer_not_full(e);
    c->get_connection_manager()->icontext_send_buffer_not_full(e);
    c->get_reliability()->icontext_send_buffer_not_full(e);
}

bool SimpleTCP::is_connected(Socket* s) {
    IContextContainer* c = get_context(s);
    return c->get_connection_manager()->is_connected(s);
}

void SimpleTCP::send_network_packet(Socket* s, WiFuPacket* p) {
    TCPPacket* packet = (TCPPacket*) p;
    NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(e);
}
