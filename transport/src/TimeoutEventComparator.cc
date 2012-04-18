#include "TimeoutEventComparator.h"

bool TimeoutEventComparator::operator()(Event * const& t1, Event * const& t2) {
    TimeoutEvent* left = (TimeoutEvent*) t1;
    TimeoutEvent* right = (TimeoutEvent*) t2;

    struct timespec& a = left->get_timeout_time();
    struct timespec& b = right->get_timeout_time();

    if (a.tv_sec < b.tv_sec) return true;
    if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) return true;
    return false;
}
