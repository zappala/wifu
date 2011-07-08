/* 
 * File:   SocketOptions.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 2:51 PM
 */

#ifndef SOCKETOPTIONS_H
#define	SOCKETOPTIONS_H

#include "GarbageCollector.h"
#include "defines.h"
#include <map>
#include <iostream>
using namespace std;

class SocketOptions : public gc {
public:

    /**
     * Constructor
     */
    SocketOptions();

    /**
     * Destructor
     */
    virtual ~SocketOptions();

    /**
     * Inserts the key value pair into the map, overwriting any existing values
     * @param key A pair of socket option level and name
     * @param value A pair of socket option value and value-length
     */
    void insert(pair<int, int> key, pair<gcstring, socklen_t> value);

    /**
     * Gets the value and value-length for key if it exists
     * @param key A pair of socket option level and name
     * @return The value and value-length associated with key.  If key is not found, make_pair("", 0) is returned.
     */
    pair<gcstring, socklen_t> get(pair<int, int> key);

    /**
     * Removes the key and value associated with key from the map
     * @param key A pair of socket option level and name
     */
    void remove(pair<int, int> key);

private:

    // Key:   pair<level, name>
    // Value: pair<value, value-length>
    map< pair<int, int>, pair<gcstring, socklen_t>, std::less<pair<int, int> >, gc_allocator<pair<pair<int, int>, pair<gcstring, socklen_t> > > > map_;
};

#endif	/* SOCKETOPTIONS_H */

