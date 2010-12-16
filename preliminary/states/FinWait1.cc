#include "FinWait1.h"
#include "Closing.h"
#include "FinWait2.h"

FinWait1::FinWait1() {

}

FinWait1::~FinWait1() {

}

void FinWait1::receive(Connection * c, Packet * p) {
    //TODO: Ensure Receive FIN
    //TODO: Send ACK
    c->setState(new Closing());

    //TODO: Ensure Receipt of ACK of FIN
    //TODO: No action
    c->setState(new FinWait2());
}
