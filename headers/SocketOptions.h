/* 
 * File:   SocketOptions.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 2:51 PM
 */

#ifndef SOCKETOPTIONS_H
#define	SOCKETOPTIONS_H

#include "GarbageCollector.h"
#include <string>
#include <map>

using namespace std;

class SocketOptions : public gc {
public:
    SocketOptions();
    virtual ~SocketOptions();

    void insert(pair<int, int> key, string& value);
    string get(pair<int, int> key);
    void remove(pair<int, int> key);

private:
    map< pair<int, int>, string> map_;
};

#endif	/* SOCKETOPTIONS_H */

