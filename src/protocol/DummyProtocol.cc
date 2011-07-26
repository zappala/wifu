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

    s->get_receive_buffer().append((const char*) e->get_data(), e->data_length());

    // Respond to the sending socket
    ResponseEvent* response = new ResponseEvent(source, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(e->data_length()));
    response->put(ERRNO, Utils::itoa(0));
    dispatch(response);

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
    int buffer_size = e->get_receive_buffer_size();

    // TODO: change this to use gcstring::data() so we avoid the copy in substr
    gcstring data = s->get_receive_buffer().substr(0, buffer_size);
    int length = data.size();
    s->get_receive_buffer().erase(0, length);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}