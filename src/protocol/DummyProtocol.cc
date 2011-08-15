#include "protocol/DummyProtocol.h"

DummyProtocol::DummyProtocol(int protocol, IContextContainerFactory* factory) : TCPTahoe(protocol, factory) {

}

DummyProtocol::~DummyProtocol() {

}

DummyProtocol& DummyProtocol::instance() {
    static DummyProtocol instance_;
    return instance_;
}

void DummyProtocol::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    set_socket_buffer_sizes(e->get_socket(), DUMMY_BUFFER_SIZE);
    this->TCPTahoe::icontext_socket(q, e);
}

void DummyProtocol::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
    set_socket_buffer_sizes(e->get_socket(), DUMMY_BUFFER_SIZE);
    this->TCPTahoe::icontext_new_connection_initiated(q, e);
}

void DummyProtocol::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {

    // CAREFUL: There are TWO sockets in action here (because we immediately receive)
    Socket* source = e->get_socket();

    AddressPort* local = source->get_remote_address_port();
    AddressPort* remote = source->get_local_address_port();

    Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

    s->get_receive_buffer().append((const char*) e->get_data(), e->get_data_length());

    // Respond to the sending socket
    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_default_length();
    response_event->set_destination(e->get_source());
    response_event->set_errno(0);
    response_event->set_message_type(e->get_message_type());
    response_event->set_return_value(e->get_data_length());
    response_event->set_socket(source);
    response_event->set_fd(e->get_fd());
    //    ResponseEvent* response = new ResponseEvent(source, e->get_name(), e->get_map()[FILE_STRING]);
    //    response->put(RETURN_VALUE_STRING, Utils::itoa(e->data_length()));
    //    response->put(ERRNO, Utils::itoa(0));
    dispatch(response_event);

    // Try to receive on the receiving socket
    DummyProtocolIContextContainer* container = (DummyProtocolIContextContainer*) map_.find(s)->second;
    if (container->get_receive_event() && !s->get_receive_buffer().empty()) {
        dispatch_received_data(q, container->get_receive_event());
        container->set_receive_event(0);
    }
}

void DummyProtocol::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    Socket* s = e->get_socket();
    if (!s->get_receive_buffer().empty()) {
        dispatch_received_data(q, e);
    } else {
        DummyProtocolIContextContainer* container = (DummyProtocolIContextContainer*) map_.find(s)->second;
        assert(!container->get_receive_event());
        container->set_receive_event(e);
    }
}

void DummyProtocol::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();
    if (p->is_tcp_fin() && !s->get_receive_buffer().empty()) {
        DummyProtocolIContextContainer* c = (DummyProtocolIContextContainer*) map_.find(s)->second;
        c->set_saved_fin(e);
        return;
    }
    TCPTahoe::icontext_receive_packet(q, e);
}

void DummyProtocol::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    DummyProtocolIContextContainer* c = (DummyProtocolIContextContainer*) map_.find(s)->second;
    if (c->get_saved_fin() && s->get_receive_buffer().empty()) {
        dispatch(c->get_saved_fin());
    }
}

void DummyProtocol::set_socket_buffer_sizes(Socket* s, int size) {
    s->get_send_buffer().reserve(size);
    s->get_receive_buffer().reserve(size);
    s->get_resend_buffer().reserve(size);
}

void DummyProtocol::dispatch_received_data(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    Socket* s = e->get_socket();
    DummyProtocolIContextContainer* c = (DummyProtocolIContextContainer*) map_.find(s)->second;
    size_t buffer_size = e->get_data_length();

    RecvFromResponseEvent* response = (RecvFromResponseEvent*) ObjectPool<ResponseEvent>::instance().get();
    response->set_socket(s);
    response->set_message_type(e->get_message_type());
    response->set_fd(e->get_fd());
    response->set_destination(e->get_source());
    response->set_addr(s->get_remote_address_port()->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    // done in set length
    //response->set_return_value(length);
    response->set_errno(0);
    int length = min(s->get_receive_buffer().size() - c->get_receive_index(), buffer_size);
    response->set_return_buffer((unsigned char*) s->get_receive_buffer().data(), length);

//    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);

//    response->put(BUFFER_STRING, s->get_receive_buffer().substr(c->get_receive_index(), buffer_size));
//    int length = response->get(BUFFER_STRING)->size();
    //s->get_receive_buffer().erase(0, length);
    c->set_receive_index(c->get_receive_index() + length);
    if (c->get_receive_index() >= s->get_receive_buffer().size()) {
        s->get_receive_buffer().clear();
        c->set_receive_index(0);
        //cout << "Calling clear" << endl;
    }

//    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
//    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
//    response->put(RETURN_VALUE_STRING, Utils::itoa(length));
//    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}