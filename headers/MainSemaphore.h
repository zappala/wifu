/* 
 * File:   MainSemaphore.h
 * Author: rbuck
 *
 * Created on December 2, 2010, 10:42 AM
 */

#ifndef _MAINSEMAPHORE_H
#define	_MAINSEMAPHORE_H

#include "Semaphore.h"


class MainSemaphore : public Semaphore {
public:

    virtual ~MainSemaphore() {

    }

    static MainSemaphore& instance() {
        static MainSemaphore instance_;
        return instance_;
    }


private:
    MainSemaphore() : Semaphore() {

    }
};

#endif	/* _MAINSEMAPHORE_H */

