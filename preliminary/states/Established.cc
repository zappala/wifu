#include "Established.h"
#include "FinWait1.h"
#include "CloseWait.h"

Established::Established() {

}

Established::~Established() {

}

void Established::close(Connection * c) {
    //TODO: Send FIN
    c->setState(new FinWait1());
}

void Established::receive(Connection * c, Packet * p) {
    //TODO: Ensure Packet is FIN
    //TODO: Send ACK
    c->setState(new CloseWait());

    //TODO: This is likely where all packets will be received;
    //TODO: i.e. the above code/notes is for transition out of this state,
    //TODO: not for the processing of packets
}
