/* 
 * File:   CacheMap.h
 * Author: rbuck
 *
 * Created on April 1, 2011, 3:06 PM
 */

#ifndef _CACHEMAP_H
#define	_CACHEMAP_H

#include "Cache.h"
#include "Socket.h"
#include "Semaphore.h"
#include <map>

using namespace std;

class CacheMap {
public:
    static CacheMap& instance();

    virtual ~CacheMap();

    Cache* get(Socket* s);

    void put(Socket* s, Cache* c);

    int size();

    bool empty();

    void remove(Socket* s);

    

private:
    CacheMap();

    map<Socket*, Cache*, std::less<Socket*>, gc_allocator<std::pair<Socket*, Cache*> > > map_;
    Semaphore mutex_;
};

#endif	/* _CACHEMAP_H */

