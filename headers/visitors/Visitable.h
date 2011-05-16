/* 
 * File:   Visitable.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:35 AM
 */

#ifndef VISITABLE_H
#define	VISITABLE_H

#include "GarbageCollector.h"
#include "Visitor.h"

/**
 * Inherting from this class makes the child class visitable.
 * 
 * Namely, the child class must implement the accept so that it
 * may be visited.
 *
 */
class Visitable : public gc {
public:

    /**
     * Constructor
     */
    Visitable();

    /**
     * Does nothing special
     */
    virtual ~Visitable();

    /**
     * Accepts a Visitor which will perform some operation on this Visitable class
     * @param v Pointer to a Visitor
     * @see Visitor
     */
    virtual void accept(Visitor* v) = 0;
};

#endif	/* VISITABLE_H */

