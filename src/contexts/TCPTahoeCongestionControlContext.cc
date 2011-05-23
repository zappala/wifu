#include "contexts/TCPTahoeCongestionControlContext.h"



TCPTahoeCongestionControlContext::TCPTahoeCongestionControlContext(u_int32_t iss) : WindowContext(iss) {
    set_state(new DummyCongestionController());
}

TCPTahoeCongestionControlContext::~TCPTahoeCongestionControlContext() {
    
}
