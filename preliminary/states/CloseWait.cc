#include "CloseWait.h"
#include "LastAck.h"

CloseWait::CloseWait() {

}

CloseWait::~CloseWait() {

}

void CloseWait::close(Connection * c) {
    //TODO: Send FIN
    c->setState(new LastAck());
}