/* 
 * File:   TimeoutEventManagerSingleton.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 9:36 AM
 */

#ifndef _TIMEOUTEVENTMANAGERSEMAPHORE_H
#define	_TIMEOUTEVENTMANAGERSEMAPHORE_H

#include "BinarySemaphore.h"

/**
 * Singleton class which internally is really a BinarySemaphore.
 */
class TimeoutEventManagerSemaphore : public BinarySemaphore {
private:

    /**
     * Create a TimeoutEventManagerSemaphore object.
     */
    TimeoutEventManagerSemaphore();
public:

    /**
     * Clean up this TimeoutEventManagerSemaphore object.
     */
    virtual ~TimeoutEventManagerSemaphore();

    /**
     * @return A reference to the static instance of this TimeoutEventManagerSemaphore object.
     */
    static TimeoutEventManagerSemaphore & instance();
};


#endif	/* _TIMEOUTEVENTMANAGERSEMAPHORE_H */
