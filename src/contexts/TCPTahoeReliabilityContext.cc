#include "contexts/TCPTahoeReliabilityContext.h"


TCPTahoeReliabilityContext::TCPTahoeReliabilityContext() {
    set_state(new TCPTahoeReliabilityState());
}

TCPTahoeReliabilityContext::~TCPTahoeReliabilityContext() {
    
}
