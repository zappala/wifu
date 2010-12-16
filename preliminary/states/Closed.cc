#include "Closed.h"
#include "SynSent.h"
#include "Listen.h"

Closed::Closed() {

}

Closed::~Closed() {
    
}

void Closed::passiveOpen(Connection * c) {
    //TODO: Create TCB
    c->setState(new Listen());
}

void Closed::activeOpen(Connection * c) {
    //TODO: Create TCB
    //TODO: Send SYN
    c->setState(new SynSent());
}
