/* 
 * File:   timer.h
 * Author: rbuck
 *
 * Created on April 5, 2010, 2:19 PM
 */

#ifndef _TIMER_H
#define	_TIMER_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <cstring>

#include <iostream>
#include <assert.h>

#define MILLION 1000000

class Timer {
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    void update_start();
    void update_stop();

    void adjust_start_time(int microseconds);
    void adjust_stop_time(int microseconds);

    u_int64_t get_start_time();
    u_int64_t get_stop_time();

    void set_start_time_to_be_stop_time();

    u_int64_t get_duration_microseconds();
    double get_duration_seconds();

private:
    struct timeval start_time;
    struct timeval stop_time;

    bool start_set;
    bool stop_set;

    void adjust_timeval(struct timeval* val, int microseconds);

    inline u_int64_t to_microseconds(struct timeval* val) {
        u_int64_t microseconds = val->tv_sec;
        microseconds *= MILLION;
        microseconds += val->tv_usec;
        return microseconds;
    }
};

#endif	/* _TIMER_H */

