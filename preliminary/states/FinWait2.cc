#include "FinWait2.h"
#include "TimeWait.h"

FinWait2::FinWait2() {

}

FinWait2::~FinWait2() {

}

void FinWait2::receive(Connection * c, Packet * p) {
    //TODO: Ensure Receive FIN
    //TODO: Send ACK
    c->setState(new TimeWait());
}