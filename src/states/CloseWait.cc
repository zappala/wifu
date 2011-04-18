#include "states/CloseWait.h"

CloseWait::CloseWait() {

}

CloseWait::~CloseWait() {

}

void CloseWait::enter(Context* c) {

}

void CloseWait::exit(Context* c) {

}

void CloseWait::close(Context* c) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}
