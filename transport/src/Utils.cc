#include "Utils.h"
#include "exceptions/IOError.h"

void Utils::get_timespec_future_time(int seconds, long int nanoseconds, struct timespec* ts) {
    assert(seconds >= 0);
    assert(nanoseconds >= 0);

    // Can we get better precision with real-time clock?
    // Needs to be value from epoch (not relative time)
    clock_gettime(CLOCK_REALTIME, ts);

    ts->tv_nsec += nanoseconds;
    ts->tv_sec += seconds;

    while (ts->tv_nsec >= NANOSECONDS_IN_SECONDS) {
        ts->tv_sec += 1;
        ts->tv_nsec -= NANOSECONDS_IN_SECONDS;
    }
}

/**
 * Gets the current time in microseconds.
 * This will wrap approximately every 1.193046471 hours (we will need to check for this when comparing).
 * @return The current time
 */
u_int32_t Utils::get_current_time_microseconds_32() {
    return get_current_time_microseconds_64();
}

u_int64_t Utils::get_current_time_microseconds_64() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u_int64_t microseconds = tv.tv_sec;
    microseconds *= 1000000;
    microseconds += tv.tv_usec;
    return microseconds;
}

gcstring Utils::itoa(int i) {
    char buf[20];
    sprintf(buf, "%d", i);
    return buf;
}

// TODO: pass in a vector by reference to avoid the copy
vector<gcstring, gc_allocator<gcstring> > Utils::read_file(gcstring& file) {
    gcstring s;
    vector<gcstring, gc_allocator<gcstring> > result;
    ifstream infile;

    try {
        infile.open(file.c_str());
    }    catch (ifstream::failure e) {
        throw IOError();
    }

    if (!infile.is_open()) {
        throw IOError();
    }

    while (!infile.eof()) {
        getline(infile, s);

        // Ignore empty lines
        if (s.empty()) {
            continue;
        }

        // Ignore comments
        if (s[0] == '#') {
            continue;
        }

        result.push_back(s);
    }
    infile.close();
    return result;
}

vector<gcstring, gc_allocator<gcstring> > Utils::tokenize(gcstring& line, gcstring& delimiters) {

    vector<gcstring, gc_allocator<gcstring> > result;
    char* current = strtok((char*) line.c_str(), delimiters.c_str());
    while (current != NULL) {
        result.push_back(current);
        current = strtok(NULL, delimiters.c_str());
    }

    return result;
}