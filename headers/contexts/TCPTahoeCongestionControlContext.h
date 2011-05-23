/* 
 * File:   TCPTahoeCongestionControlContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOECONGESTIONCONTROLCONTEXT_H
#define	TCPTAHOECONGESTIONCONTROLCONTEXT_H

#include "WindowContext.h"
//#include "states/SlowStart.h"

// TODO: replace this with an actual state machine
#include "states/DummyCongestionController.h"

class TCPTahoeCongestionControlContext : public WindowContext {
public:
    TCPTahoeCongestionControlContext(u_int32_t iss);
    virtual ~TCPTahoeCongestionControlContext();
    
private:
};

#endif	/* TCPTAHOECONGESTIONCONTROLCONTEXT_H */

