#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context() {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

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

void ReliabilityContext::connection_established(Socket* s) {
    get_state()->connection_established(this, s);
}

void ReliabilityContext::close() {
    get_state()->close(this);
}

void ReliabilityContext::receive(Socket* s, Packet* p) {
    get_state()->receive(this, s, p);
}

void ReliabilityContext::send(Socket* s, Packet* p) {
    get_state()->send(this, s, p);
}