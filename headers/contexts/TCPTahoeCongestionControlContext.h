/* 
 * File:   TCPTahoeCongestionControlContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOECONGESTIONCONTROLCONTEXT_H
#define	TCPTAHOECONGESTIONCONTROLCONTEXT_H

#include "Context.h"
//#include "states/SlowStart.h"

// TODO: replace this with an actual state machine
#include "states/DummyCongestionController.h"

class TCPTahoeCongestionControlContext : public Context {
public:
    TCPTahoeCongestionControlContext();
    virtual ~TCPTahoeCongestionControlContext();
    
private:
};

#endif	/* TCPTAHOECONGESTIONCONTROLCONTEXT_H */

