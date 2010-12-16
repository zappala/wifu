#include "Listen.h"
#include "Closed.h"
#include "SynSent.h"
#include "SynReceived.h"

Listen::Listen() {

}

Listen::~Listen() {

}

void Listen::close(Connection * c) {
    //TODO: Delete TCB
    c->setState(new Closed());
}

void Listen::send(Connection * c) {
    //TODO: Send SYN
    c->setState(new SynSent());
}

void Listen::receive(Connection * c, Packet * p) {
    //TODO: Ensure Packet is SYN
    //TODO: Send SYNACK
    c->setState(new SynReceived());
}