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

};

#endif	/* SIMPLETCP_H */

