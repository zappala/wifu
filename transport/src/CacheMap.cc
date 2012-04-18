#include "CacheMap.h"

CacheMap& CacheMap::instance() {
    static CacheMap instance_;
    return instance_;
}

CacheMap::~CacheMap() {

}

Cache* CacheMap::get(Socket* s) {
    mutex_.wait();
    map<Socket*, Cache*, std::less<Socket*>, gc_allocator<std::pair<Socket*, Cache*> > >::iterator itr = map_.find(s);
    Cache* c = itr == map_.end() ? NULL : itr->second;
    mutex_.post();
    return c;
}

void CacheMap::put(Socket* s, Cache* c) {
    mutex_.wait();
    map_[s] = c;
    mutex_.post();
}

int CacheMap::size() {
    mutex_.wait();
    int size = map_.size();
    mutex_.post();
    return size;
}

bool CacheMap::empty() {
    mutex_.wait();
    bool empty = map_.empty();
    mutex_.post();
    return empty;
}

void CacheMap::remove(Socket* s) {
    mutex_.wait();
    map_.erase(s);
    mutex_.post();
}

CacheMap::CacheMap() {
    mutex_.init(1);
}
