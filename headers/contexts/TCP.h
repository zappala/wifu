/* 
 * File:   TCP.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef TCP_H
#define	TCP_H

#include "Protocol.h"
#include "ReliabilityContext.h"
#include "ConnectionManagerContext.h"
#include "CongestionControlContext.h"

class TCP : public Protocol {
public:
    TCP() : Protocol() {
        get_contexts().push_back(new ReliabilityContext());
        get_contexts().push_back(new ConnectionManagerContext());
        get_contexts().push_back(new CongestionControlContext());
    }
};

#endif	/* TCP_H */

