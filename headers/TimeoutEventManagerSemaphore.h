/* 
 * File:   TimeoutEventManagerSingleton.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 9:36 AM
 */

#ifndef _TIMEOUTEVENTMANAGERSEMAPHORE_H
#define	_TIMEOUTEVENTMANAGERSEMAPHORE_H

#include "BinarySemaphore.h"

class TimeoutEventManagerSemaphore : public BinarySemaphore {
private:
    TimeoutEventManagerSemaphore();
public:
    virtual ~TimeoutEventManagerSemaphore();
    static TimeoutEventManagerSemaphore & instance();
};


#endif	/* _TIMEOUTEVENTMANAGERSEMAPHORE_H */
