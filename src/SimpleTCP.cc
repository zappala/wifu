#include "SimpleTCP.h"

SimpleTCP::SimpleTCP() : Protocol(SIMPLE_TCP) {
}

void SimpleTCP::save_socket(Socket* s) {
    map_[s] = new IContextContainer();
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

    c->get_congestion_control()->listen(e);
    c->get_connection_manager()->listen(e);
    c->get_reliability()->listen(e);
}

void SimpleTCP::receive_packet(NetworkReceivePacketEvent* e) {
    IContextContainer* c = get_context(e->get_socket());

    c->get_congestion_control()->receive_packet(e);
    c->get_connection_manager()->receive_packet(e);
    c->get_reliability()->receive_packet(e);
}

void SimpleTCP::send_packet(SendPacketEvent* e) {
    cout << "SimpleTCP::send_packet()" << endl;
    Socket* s = e->get_socket();
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->send_packet(e);
    c->get_connection_manager()->send_packet(e);
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
    Socket* new_socket = e->get_new_socket();
    save_socket(new_socket);
    IContextContainer* c = get_context(new_socket);

    c->get_congestion_control()->new_connection_established(e);
    c->get_connection_manager()->new_connection_established(e);
    c->get_reliability()->new_connection_established(e);

}

void SimpleTCP::close() {
}

void SimpleTCP::timer_fired_event(TimerFiredEvent* e) {
    cout << "In SimpleTCP::timer_fired()\n";
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
    //    c->get_connection_manager()->resend_packet(s, p);

    // TODO: I think reliability needs to know about a resend (so it can start up another timer)
    c->get_reliability()->resend_packet(e);

    send_network_packet(s, e->get_packet());
}

void SimpleTCP::send_network_packet(Socket* s, WiFuPacket* p) {
    TCPPacket* packet = (TCPPacket*) p;
    cout << "SimpleTCP::send_network_packet(): SYN: " << packet->is_tcp_syn() << " ACK: " << packet->is_tcp_ack() << endl;
    NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(e);
}
