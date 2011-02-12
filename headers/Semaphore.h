/* 
 * File:   Semaphore.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 8:49 AM
 */

#ifndef _MYSEMAPHORE_H
#define	_MYSEMAPHORE_H

#include <semaphore.h>
#include <time.h>
#include <iostream>
#include <errno.h>
#include "GarbageCollector.h"

using namespace std;

/**
 * This is a counting semaphore.  This class simply wraps a a sem_t object.
 */
class Semaphore : public gc {
public:

    /**
     * Creates a Semaphore object.  Does not initialize the count; call init() to do so.
     */
    Semaphore();

    /**
     * Cleans up this Semaphore object.
     */
    virtual ~Semaphore();

    /**
     * Initializes the internal semaphore count to be value.
     *
     * @param value The initial value to set this Semaphore's internal count.
     */
    virtual void init(int value);

    /**
     * If the internal count is greater than 0, decrements the count and returns.
     * Otherwise, this call blocks until post() is called (which increments the internal counter).
     *
     * @see timed_wait()
     */
    void wait(void);

    /**
     * If the internal count is greater than 0, decrements the count and returns.
     * If the internal count is equal to 0, this method simply returns.
     */
    void try_wait(void);

    /**
     * Increment the internal count by one.
     */
    virtual void post(void);

    /**
     * Nearly the same as wait(). The difference is that if this call is going
     * to block, ts specifies an absolute time by which it will unblock and
     * return. If it can unblock before that time it does so and returns like
     * normal.
     *
     * @param ts An absolute time (in the future) which represents the latest point which this call will block.
     * @return True if this method timed out, false if this method successfully decremented the internal count.
     * @see wait()
     */
    bool timed_wait(struct timespec * ts);

    /**
     * @return The current internal count of this Semaphore.
     */
    int get_value(void);

protected:

    /**
     * Actual semaphore object which this class wraps.
     */
    sem_t sem_;
};


#endif	/* _MYSEMAPHORE_H */

