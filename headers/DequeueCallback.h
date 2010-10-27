/* 
 * File:   DequeueCallback.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 11:19 AM
 */

#ifndef _DEQUEUECALLBACK_H
#define	_DEQUEUECALLBACK_H

#include "Event.h"

class DequeueCallback {
public:
    virtual void process(Event * event) = 0;
};

#endif	/* _DEQUEUECALLBACK_H */

