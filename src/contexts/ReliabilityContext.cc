#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context() {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

}

void ReliabilityContext::socket(Socket* s) {
    get_state()->socket(this, s);
}

void ReliabilityContext::bind(Socket* s, AddressPort* ap) {
    get_state()->bind(this, s, ap);
}

void ReliabilityContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void ReliabilityContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void ReliabilityContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void ReliabilityContext::new_connection_established(Socket* s) {
    get_state()->new_connection_established(this, s);
}

void ReliabilityContext::close() {
    get_state()->close(this);
}

void ReliabilityContext::receive(Socket* s, WiFuPacket* p) {
    get_state()->receive(this, s, p);
}

void ReliabilityContext::send_packet(Socket* s, WiFuPacket* p) {
    get_state()->send_packet(this, s, p);
}