/* 
 * File:   Observer.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 2:24 PM
 */

#ifndef _OBSERVER_H
#define	_OBSERVER_H

#include "Observable.h"

class Observer {
public:
    Observer();
    virtual ~Observer();
    virtual void update(class Observable* o) = 0;
};

#endif	/* _OBSERVER_H */

