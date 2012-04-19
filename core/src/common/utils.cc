#include "utils.h"

// gets the current time since the epoch in microseconds
u_int64_t get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u_int64_t microseconds = tv.tv_sec;
    microseconds *= MILLION;
    microseconds += tv.tv_usec;
    return microseconds;
}

// gets a string representation of the current time, at a resoultion of seconds
string get_asc_time() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    ostringstream time;
    time << 1900+tm->tm_year << "-" << setw(2) << setfill('0') << tm->tm_mon << "-" << tm->tm_mday << "-"
         << tm->tm_hour << "-" << tm->tm_min << "-" << tm->tm_sec;
    return time.str();
}
