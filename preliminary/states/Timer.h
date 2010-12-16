/* 
 * File:   Timer.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 10:42 AM
 */

#ifndef _TIMER_H
#define	_TIMER_H

#include "TimerCallback.h"

class Timer {
public:
    Timer(int microseconds, TimerCallback * callback) {
        callback_ = callback;
        microseconds_ = microseconds;
    }
    
    void start() {

    }

    void cancel() {
        
    }

private:
    TimerCallback * callback_;
    int microseconds_;
};


#endif	/* _TIMER_H */

