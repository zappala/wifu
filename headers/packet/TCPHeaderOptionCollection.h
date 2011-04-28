/* 
 * File:   TCPHeaderOptionCollection.h
 * Author: rbuck
 *
 * Created on April 28, 2011, 10:31 AM
 */

#ifndef TCPHEADEROPTIONCOLLECTION_H
#define	TCPHEADEROPTIONCOLLECTION_H

#include <vector>
#include <sys/types.h>
#include "GarbageCollector.h"
#include "TCPHeaderOption.h"
#include "visitors/Visitable.h"

using namespace std;

class TCPHeaderOptionCollection : public gc, public Visitable {
public:
    TCPHeaderOptionCollection();
    virtual ~TCPHeaderOptionCollection();

    void insert(TCPHeaderOption* option);
    TCPHeaderOption* remove(u_int8_t kind);
    bool contains(u_int8_t kind);

    u_int8_t get_padded_length();
    unsigned char* get_data();

    void accept(Visitor* v);
    

private:
    vector<TCPHeaderOption*> options_;
};

#endif	/* TCPHEADEROPTIONCOLLECTION_H */

