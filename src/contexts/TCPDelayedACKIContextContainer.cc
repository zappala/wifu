#include "contexts/TCPDelayedACKIContextContainer.h"

TCPDelayedACKIContextContainer::TCPDelayedACKIContextContainer() : TCPTahoeIContextContainer() {
    u_int32_t iss = 1;
    set_reliability(new TCPDelayedACKReliabilityContext(iss));
}

TCPDelayedACKIContextContainer::~TCPDelayedACKIContextContainer() {

}