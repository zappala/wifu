/* 
 * File:   Event.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:46 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H

#include "GarbageCollector.h"
#include "IModule.h"

class IModule;

/**
 * Represents an Event which IModule objects will receive and react to.
 */
class Event : public gc {
public:

    /**
     * Constructs an Event object.
     *
     * @param socket The socket to which to apply this Event to.
     */
    Event(int socket);

    /**
     * Cleans up this Event object.
     */
    virtual ~Event();

    /**
     * Will call a specific method on m depending on the type of this Event.
     *
     * @param m The module which to call a specific method on.
     * @see IModule
     */
    virtual void execute(IModule* m) = 0;

    /**
     * @return A reference to the socket.
     */
    int & get_socket();

    /**
     * Default implemenation of less than.
     *
     * @param rhs The "right hand side" of the expression A < rhs, where A is this Event.
     * @return True in each case.  Must be overridden in child classes and satisfy the above expression.
     */
    virtual bool less_than(Event* rhs);
private:

    /**
     * ID of the Socket which is associated with this Event.
     */
    int socket_;

};

/**
 * Comparator used to compare two Event pointers.
 */
class EventComparator {
public:

    /**
     * Compares two Event pointers (actually references to Event pointers).
     * Will return t1 < t2.
     *
     * @param t1 The RHS of the expression t1 < t2
     * @param t2 the LHS of the expression t1 < t1
     * @return True if t1 < t2, false otherwise.
     */
    bool operator()(Event*& t1, Event*& t2) {
        return t1->less_than(t2);
    }
};



#endif	/* _EVENT_H */

