/* 
 * File:   Observable.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 2:27 PM
 */

#ifndef _OBSERVABLE_H
#define	_OBSERVABLE_H

#include "Observer.h"
#include <set>

using namespace std;

/**
 * Base class to be used to make a child class observable in the context of the observer pattern.
 * For information on the observer pattern see: http://sourcemaking.com/design_patterns/observer
 *
 * When a class inherits from Observable, it can then notify Observer classes of internal changes so they can then get the changes.
 *
 * @see Observer
 */
class Observable {
private:

    /**
     * The collection of Observers to notify when Observable::notify() is called.
     */
    set<class Observer*> observers_;

    /**
     * Iterator for observers_
     */
    set<class Observer*>::iterator itr_;

public:

    /**
     * Constructor.
     */
    Observable();

    /**
     * Destructor.
     */
    virtual ~Observable();

    /**
     * Adds an Observer to the collection of Observer object to notify.
     * @param o Observer to add to the collection.
     */
    void add_observer(class Observer* o);

    /**
     * Notifies each Observer of changes to this Observable object's state.
     * This is done by calling the Observer::update() function on each Observer.
     */
    void notify();

};

#endif	/* _OBSERVABLE_H */

