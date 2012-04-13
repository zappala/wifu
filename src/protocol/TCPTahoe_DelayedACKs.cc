#include "protocol/TCPTahoe_DelayedACKs.h"
#include "MockNetworkInterface.h"

TCPDelayedACK::TCPDelayedACK(int protocol, IContextContainerFactory* factory) : TCPTahoe(protocol, factory) {
}

TCPDelayedACK::~TCPDelayedACK() {
}

TCPDelayedACK& TCPDelayedACK::instance() {
    static TCPDelayedACK instance_;
    return instance_;
}
