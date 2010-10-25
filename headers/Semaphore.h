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

using namespace std;

class Semaphore {
public:

    Semaphore();
    virtual ~Semaphore();

    virtual void init(int value);
    void wait(void);
    virtual void post(void);
    void timed_wait(struct timespec * ts);
    int get_value();

protected:
    sem_t sem_;
};


#endif	/* _MYSEMAPHORE_H */

