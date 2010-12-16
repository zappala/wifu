#include "Closing.h"
#include "TimeWait.h"

Closing::Closing() {

}

Closing::~Closing() {

}

void Closing::receive(Connection * c, Packet * p) {
    //TODO: Ensure receipt of ACK of FIN
    //TODO: No action
    c->setState(new TimeWait());
}