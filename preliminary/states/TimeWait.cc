#include "TimeWait.h"
#include "Closed.h"

TimeWait::TimeWait() {

}

TimeWait::~TimeWait() {

}

void TimeWait::timeout(Connection * c) {
    //TODO: Ensure Timeout==2MSL
    //TODO: Delete TCB
    c->setState(new Closed());
}
