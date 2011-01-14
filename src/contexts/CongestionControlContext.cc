#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context() {
    set_state(new SlowStart());
}

void CongestionControlContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void CongestionControlContext::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void CongestionControlContext::accept(Socket* s) {
    get_state()->accept(this, s);
}

void CongestionControlContext::close() {
    get_state()->close(this);
}

void CongestionControlContext::receive(Socket* s, Packet* p) {
    get_state()->receive(this, s, p);
}

void CongestionControlContext::send(Socket* s, Packet* p) {
    cout << "Congestion Control Send" << endl;
    get_state()->send(this, s, p);
}

bool CongestionControlContext::is_open() {
    return get_state()->is_open();
}
