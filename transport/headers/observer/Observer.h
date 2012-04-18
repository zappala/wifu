/* 
 * File:   Observer.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 2:24 PM
 */

#ifndef _OBSERVER_H
#define	_OBSERVER_H

#include "GarbageCollector.h"
#include "Observable.h"

using namespace std;

/**
 * Base class to make a child class an observer in the context of the observer pattern.
 * For information on the observer pattern see: http://sourcemaking.com/design_patterns/observer
 *
 * When a class inherits from Observer, it can then be added to an Observable object's collection to be notified of changes to the Observable object.
 *
 * @see Observable
 */
class Observer : public gc {
public:
    
    /**
     * Constructor.
     */
    Observer();

    /**
     * Destructor.
     */
    virtual ~Observer();

    /**
     * This function is called when o has been modified and this Observer needs to know about it.
     *
     * @param o The Observable that has been modified.
     */
    virtual void update(class Observable* o) = 0;
};

#endif	/* _OBSERVER_H */

