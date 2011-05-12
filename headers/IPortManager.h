/* 
 * File:   IPortManager.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 5:04 PM
 */

#ifndef IPORTMANAGER_H
#define	IPORTMANAGER_H

#include "GarbageCollector.h"
#include <sys/types.h>

class IPortManager : public gc {
public:
    virtual u_int16_t get() = 0;
    virtual void remove(u_int16_t port) = 0;
    virtual void reset() = 0;
};


#endif	/* IPORTMANAGER_H */

