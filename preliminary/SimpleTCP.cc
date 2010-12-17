#include "SimpleTCP.h"
#include "Closed.h"

SimpleTCP::SimpleTCP(AddressPort& ap) : UDPSocketCallback() {
    socket_.bind_socket(ap);
    socket_.receive(this);

    local_ = new AddressPort(ap);
    remote_ = 0;

    queue_flag_.init(0);

    state_ = 0;
    set_state(new Closed());
}

SimpleTCP::~SimpleTCP() {
    if (local_) {
        delete local_;
    }

    if (remote_) {
        delete remote_;
    }

    if (state_) {
        delete state_;
    }
}

void SimpleTCP::receive(AddressPort& ap, unsigned char* buffer, size_t length) {
    SimplePacket packet(buffer, length);
    state_->receive(this, ap, packet);

}

void SimpleTCP::send(unsigned char* buffer, size_t length) {
    // fill up the Queue here,
}

void SimpleTCP::send(AddressPort* ap, SimplePacket& p) {
    socket_.send(ap->get_network_struct_ptr(), p.to_bytes(), p.get_total_length());
}

void SimpleTCP::connect(AddressPort& ap) {
    state_->activeOpen(this, ap);
}

void SimpleTCP::set_state(SimpleTCPState* state) {
    if (state_ != 0) {
        state_->exit(this);
    }
    delete state_;
    state_ = state;
    if (state_ != 0) {
        state_->enter(this);
    }
}
