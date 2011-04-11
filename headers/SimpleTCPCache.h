/* 
 * File:   SimpleTCPCache.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 12:09 PM
 */

#ifndef SIMPLETCPCACHE_H
#define	SIMPLETCPCACHE_H

#include "Cache.h"
#include "packet/WiFuPacket.h"

class SimpleTCPCache : public Cache {
public:
    SimpleTCPCache();
    
    WiFuPacket* get_packet();
    void save_packet(WiFuPacket* p);

    bool is_empty();

private:
    WiFuPacket* packet_;

};

#endif	/* SIMPLETCPCACHE_H */

