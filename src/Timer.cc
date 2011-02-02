
#include "Timer.h"

Timer::Timer() {
    start_set = false;
    stop_set = false;
}

Timer::~Timer() {

}

void Timer::start() {
    if (!start_set) {
        update_start();
    }
}

void Timer::update_start() {
    gettimeofday(&start_time, NULL);
    start_set = true;
}

void Timer::stop() {
    update_stop();
}

void Timer::update_stop() {
    gettimeofday(&stop_time, NULL);
    stop_set = true;
}

void Timer::adjust_start_time(int microseconds) {
    assert(start_set);
    adjust_timeval(&start_time, microseconds);

}

void Timer::adjust_stop_time(int microseconds) {
    assert(stop_set);
    adjust_timeval(&stop_time, microseconds);
}

void Timer::set_start_time_to_be_stop_time() {
    memcpy(&start_time, &stop_time, sizeof (struct timeval));
    stop_set = false;
}

u_int64_t Timer::get_start_time() {
    //assert(start_set);
    if (!start_set) {
        return 0;
    }

    return to_microseconds(&start_time);
}

u_int64_t Timer::get_stop_time() {
    //assert(stop_set);

    if (!stop_set) {
        return 0;
    }

    return to_microseconds(&stop_time);
}

u_int64_t Timer::get_duration_microseconds() {
    u_int64_t start_time = get_start_time();

    if (start_time == 0) {
        return 0;
    }

    u_int64_t stop_time = get_stop_time();

    stop_time -= start_time;
    return stop_time;
}

double Timer::get_duration_seconds() {
    long long int duration = get_duration_microseconds();

    int bufsize = 1000;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    sprintf(buf, "%lld", duration);

    double diff = atof(buf);
    diff /= (double) MILLION;
    return diff;
}
