#include "LastAck.h"
#include "Closed.h"

LastAck::LastAck() {

}

LastAck::~LastAck() {

}

void LastAck::receive(Connection * c, Packet * p) {
    //TODO: Ensure receipt of ACK of FIN
    //TODO: No action
    c->setState(new Closed());
}