#include "Utils.h"

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

string Utils::itoa(int i) {
    char buf[sizeof (i)*8 + 1];
    sprintf(buf, "%d", i);
    return string(buf);
}

vector<string> Utils::read_file(string& file) {
    string s;
    vector<string> result;
    ifstream infile;
    infile.open(file.c_str());
    while (!infile.eof()) {
        getline(infile, s);

        // Ignore empty lines
        if(s.empty()) {
            continue;
        }

        // Ignore comments
        if(s[0] == '#') {
            continue;
        }

        result.push_back(s);
    }
    infile.close();
    return result;
}
