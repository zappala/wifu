#include "../headers/states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
}

void Established::exit(Context* c) {

}

void Established::receive(Context* c, string& data) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}
