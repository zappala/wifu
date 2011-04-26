#include "SocketOptions.h"

SocketOptions::SocketOptions() {

}

SocketOptions::~SocketOptions() {

}

void SocketOptions::insert(pair<int, int> key, string& value) {
    map_.insert(make_pair(key, value));
}

string SocketOptions::get(pair<int, int> key) {
    map<pair<int, int>, string>::iterator itr = map_.find(key);

    if (itr != map_.end()) {
        // value is found
        return itr->second;
    }

    return "";
}

void SocketOptions::remove(pair<int, int> key) {
    map_.erase(key);
}
