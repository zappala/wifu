#include "SynReceived.h"
#include "Established.h"
#include "FinWait1.h"

SynReceived::SynReceived() {

}

SynReceived::~SynReceived() {

}

void SynReceived::receive(Connection * c, Packet * p) {
    //TODO: Receive ACK of SYN; i.e., Packet must be an ACK
    //TODO: No action
    c->setState(new Established());
}

void SynReceived::close(Connection * c) {
    //TODO: Send FIN
    c->setState(new FinWait1());
}