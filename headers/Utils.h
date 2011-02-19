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
    static void get_timespec_future_time(int, long int, struct timespec*);

    static string itoa(int);
};

#endif	/* _UTILS_H */

