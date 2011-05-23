#include "contexts/TCPTahoeCongestionControlContext.h"



TCPTahoeCongestionControlContext::TCPTahoeCongestionControlContext() {
    set_state(new DummyCongestionController());
}

TCPTahoeCongestionControlContext::~TCPTahoeCongestionControlContext() {
    
}
