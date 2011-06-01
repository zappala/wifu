/* 
 * File:   PriorityEventComparator.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 3:34 PM
 */

#ifndef PRIORITYEVENTCOMPARATOR_H
#define	PRIORITYEVENTCOMPARATOR_H

#include "events/Event.h"

class PriorityEventComparator {
public:
    /**
     * Compares two Event pointers according to their Priority.
     *
     * @param t1 The RHS of the expression t1 < t2
     * @param t2 the LHS of the expression t1 < t1
     * @return True if t1 < t2 (t1's priority is HIGHER than t2), false otherwise
     */
    bool operator()(Event* const& t1, Event* const& t2);
};

#endif	/* PRIORITYEVENTCOMPARATOR_H */

