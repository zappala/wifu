/* 
 * File:   TimeoutEventComparator.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 11:52 AM
 */

#ifndef TIMEOUTEVENTCOMPARATOR_H
#define	TIMEOUTEVENTCOMPARATOR_H

#include "events/Event.h"
#include "events/TimeoutEvent.h"

class TimeoutEventComparator {
public:
    /**
     * Compares two Event pointers (which are TimeoutEvents).
     * Will return t1 < t2.  That is, whether t1 should timeout before t2.
     *
     * @param t1 The RHS of the expression t1 < t2
     * @param t2 the LHS of the expression t1 < t1
     * @return True if t1 < t2, false otherwise.
     */
    bool operator()(Event* const& t1, Event* const& t2);
};

#endif	/* TIMEOUTEVENTCOMPARATOR_H */

