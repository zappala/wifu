#include "../headers/states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
    cout << "Entering Established State" << endl;

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    switch(cmc->get_connection_type()) {
        case ACTIVE:
            break;
        case PASSIVE:
            break;
        default:
            break;
    }
}

void Established::exit(Context* c) {

}

void Established::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}
