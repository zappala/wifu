/* 
 * File:   Utils.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 3:26 PM
 */

#ifndef _UTILS_H
#define	_UTILS_H

#include <assert.h>
#include <time.h>
#include <string>
#include <stdio.h>

#include "defines.h"

using namespace std;

class Utils {
public:
	/**
	 * Sets the timespec struct passed in to -seconds- and -nanoseconds- into
	 * the future.
	 *
	 * @param seconds the # of seconds to set the timespec forward
	 * @param nanoseconds the # of nanoseconds to set the timespec forward
	 * @param ts pointer to the timespec to set into the future
	 */
    static void get_timespec_future_time(int seconds, long int nanoseconds, struct timespec* ts);

    static string itoa(int i);
};

#endif	/* _UTILS_H */

