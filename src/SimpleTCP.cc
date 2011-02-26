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

void SimpleTCP::socket(Socket* s) {
    save_socket(s);
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->socket(s);
    c->get_connection_manager()->socket(s);
    c->get_reliability()->socket(s);
}

void SimpleTCP::bind(Socket* s, AddressPort* ap) {
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->bind(s, ap);
    c->get_connection_manager()->bind(s, ap);
    c->get_reliability()->bind(s, ap);
}

void SimpleTCP::listen(Socket* s, int back_log) {
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->listen(s, back_log);
    c->get_connection_manager()->listen(s, back_log);
    c->get_reliability()->listen(s, back_log);
}

void SimpleTCP::receive_packet(Socket* s, WiFuPacket* p) {
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->receive_packet(s, p);
    c->get_connection_manager()->receive_packet(s, p);
    c->get_reliability()->receive_packet(s, p);
}

void SimpleTCP::send_packet(Socket* s, WiFuPacket* p) {
    cout << "SimpleTCP::send_packet()" << endl;
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->send_packet(s, p);
    c->get_connection_manager()->send_packet(s, p);
    c->get_reliability()->send_packet(s, p);

    send_network_packet(s, p);
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

void SimpleTCP::new_connection_established(Socket* s) {
    save_socket(s);
    IContextContainer* c = get_context(s);

    c->get_congestion_control()->new_connection_established(s);
    c->get_connection_manager()->new_connection_established(s);
    c->get_reliability()->new_connection_established(s);

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

void SimpleTCP::resend_packet(Socket* s, WiFuPacket* p) {
    cout << "In SimpleTCP::resend_event()\n";
    IContextContainer* c = get_context(s);

    // TODO: Does congestion control need to know about a resend?
    c->get_congestion_control()->resend_packet(s, p);

    // Probably don't want to change states when we resend
    // TODO: Does connection manager need to know about a resend?
    //    c->get_connection_manager()->resend_packet(s, p);

    // TODO: I think reliability needs to know about a resend (so it can start up another timer)
    c->get_reliability()->resend_packet(s, p);

    send_network_packet(s, p);
}

void SimpleTCP::send_network_packet(Socket* s, WiFuPacket* p) {
    TCPPacket* packet = (TCPPacket*) p;
    cout << "SimpleTCP::send_network_packet(): SYN: " << packet->is_tcp_syn() << " ACK: " << packet->is_tcp_ack() << endl;
    NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
    Dispatcher::instance().enqueue(e);
}
