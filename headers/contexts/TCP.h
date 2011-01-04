/* 
 * File:   TCP.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef TCP_H
#define	TCP_H

#include "ProtocolContext.h"
#include "ReliabilityContext.h"
#include "ConnectionManagerContext.h"
#include "CongestionControlContext.h"

class TCP : public ProtocolContext {
public:
    TCP() : ProtocolContext() {
        add_context(new ReliabilityContext());
        add_context(new ConnectionManagerContext());
        add_context(new CongestionControlContext());
    }
};

#endif	/* TCP_H */

