/* 
 * File:   BinarySemaphore.h
 * Author: rbuck
 *
 * Created on October 25, 2010, 3:22 PM
 */

#ifndef _BINARYSEMAPHORE_H
#define	_BINARYSEMAPHORE_H

#include "Semaphore.h"


class BinarySemaphore : public Semaphore {
public:
    BinarySemaphore();
    virtual ~BinarySemaphore();

    virtual void init(int value);
    virtual void post();

};

#endif	/* _BINARYSEMAPHORE_H */

