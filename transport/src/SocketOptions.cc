#include "SocketOptions.h"

SocketOptions::SocketOptions() {

}

SocketOptions::~SocketOptions() {

}

void SocketOptions::insert(pair<int, int> key, pair<gcstring, socklen_t> value) {
    map_[key] = value;
}

pair<gcstring, socklen_t> SocketOptions::get(pair<int, int> key) {
    map< pair<int, int>, pair<gcstring, socklen_t>, std::less<pair<int, int> >, gc_allocator<pair<pair<int, int>, pair<gcstring, socklen_t> > > >::iterator itr = map_.find(key);

    if (itr != map_.end()) {
        // value is found
        return itr->second;
    }

    return make_pair("", 0);
}

void SocketOptions::remove(pair<int, int> key) {
    map_.erase(key);
}
