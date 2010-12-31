/* 
 * File:   SimpleTCP.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 3:13 PM
 */

#ifndef SIMPLETCP_H
#define	SIMPLETCP_H

#include "Protocol.h"
#include "defines.h"

#include "contexts/ContextContainer.h"
#include "contexts/CongestionControlContext.h"
#include "contexts/ConnectionManagerContext.h"
#include "contexts/CongestionControlContext.h"
#include "contexts/ReliabilityContext.h"

class SimpleTCP : public Protocol {
private:

    SimpleTCP() : Protocol(SIMPLE_TCP) {
        
    }

public:

    static SimpleTCP& instance() {
        static SimpleTCP instance_;
        return instance_;
    }

    virtual ~SimpleTCP() {
        
    }

    virtual void connect(AddressPort& ap) {
        
    }

    ContextContainer* get_contexts() {
        ContextContainer* container = new ContextContainer();
        //container->add_context(new ReliabilityContext());
        container->add_context(new ConnectionManagerContext());
        //container->add_context(new CongestionControlContext());
        return container;
    }

};

#endif	/* SIMPLETCP_H */

