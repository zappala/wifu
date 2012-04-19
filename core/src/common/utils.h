#ifndef _utils_h
#define	_utils_h

// includes
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

// C++ includes
#include <iomanip>
#include <sstream>
#include <string>

// local includes
#include "defines.h"

// gets the current time since the epoch in microseconds
u_int64_t get_time();

// gets a string representation of the current time, at a resoultion of seconds
string get_asc_time();

#endif	// _utils_h
