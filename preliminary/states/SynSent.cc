#include "SynSent.h"
#include "Closed.h"
#include "Established.h"
#include "SynReceived.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::close(Connection * c) {
    //TODO: Delete TCB
    c->setState(new Closed());
}

void SynSent::receive(Connection * c, Packet * p) {
    //TODO: If Packet is SYNACK
    //TODO: Send ACK
    c->setState(new Established());

    //TODO: Else If Packet is SYN
    //TODO: Send ACK
    c->setState(new SynReceived());
}