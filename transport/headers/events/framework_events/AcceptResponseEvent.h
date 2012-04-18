/* 
 * File:   AcceptResponseEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 11:13 AM
 */

#ifndef ACCEPTRESPONSEEVENT_H
#define	ACCEPTRESPONSEEVENT_H

#include "AddressResponseEvent.h"

class AcceptResponseEvent : public AddressResponseEvent {
public:
    AcceptResponseEvent();
    virtual ~AcceptResponseEvent();
};

#endif	/* ACCEPTRESPONSEEVENT_H */

