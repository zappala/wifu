#include "contexts/TCPTahoeCongestionControlContext.h"


TCPTahoeCongestionControlContext::TCPTahoeCongestionControlContext() {
    set_state(new SlowStart());
}

TCPTahoeCongestionControlContext::~TCPTahoeCongestionControlContext() {
    
}
