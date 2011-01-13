#include "../headers/states/Established.h"

Established::Established() {

}

Established::~Established() {

}

void Established::enter(Context* c) {
    // TODO: spawn new Socket.
    cout << "Entering Established State" << endl;

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    ResponseEvent* response;
    ConnectEvent* event;

    switch(cmc->get_connection_type()) {
        case ACTIVE:
            cout << "Established::enter(), active connection" << endl;
            
            break;
        case PASSIVE:
            cout << "Established::enter(), passive connection" << endl;
            
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
