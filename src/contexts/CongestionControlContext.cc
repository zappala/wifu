#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context() {
    set_state(new SlowStart());
}

void CongestionControlContext::socket(Socket* s) {
    get_state()->socket(this, s);
}

void CongestionControlContext::bind(Socket* s, AddressPort* ap) {
    get_state()->bind(this, s, ap);
}

void CongestionControlContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void CongestionControlContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void CongestionControlContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void CongestionControlContext::new_connection_established(Socket* s) {
    get_state()->new_connection_established(this, s);
}

void CongestionControlContext::close() {
    get_state()->close(this);
}

void CongestionControlContext::receive_packet(Socket* s, WiFuPacket* p) {
    get_state()->receive_packet(this, s, p);
}

void CongestionControlContext::send_packet(Socket* s, WiFuPacket* p) {
    get_state()->send_packet(this, s, p);
}

