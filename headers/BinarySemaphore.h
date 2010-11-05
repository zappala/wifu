/* 
 * File:   BinarySemaphore.h
 * Author: rbuck
 *
 * Created on October 25, 2010, 3:22 PM
 */

#ifndef _BINARYSEMAPHORE_H
#define	_BINARYSEMAPHORE_H

#include "Semaphore.h"

/**
 * The internal value of this BinarySemaphore, will either be 0 or 1 at all times.
 * post() first calls try_wait() before actually incrementing the internal semaphhore.
 */
class BinarySemaphore : public Semaphore {
public:
    /**
     * Creates a BinarySemaphore.
     */
    BinarySemaphore();

    /**
     * Cleans up a BinarySemaphore.
     */
    virtual ~BinarySemaphore();

    /**
     * Initializes the internal semaphore count to be value if value is 0, and 1 otherwise.
     *
     * @param value The initial value to set this BinarySemaphore's internal count.
     */
    virtual void init(int value);

    /**
     * Increments the internal counter by one.
     * If the value is at one at the time of calling,
     * it is first decremented to zero, then incremented to one.
     */
    virtual void post();

};

#endif	/* _BINARYSEMAPHORE_H */

