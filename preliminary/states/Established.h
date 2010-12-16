/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:41 AM
 */

#ifndef _ESTABLISHED_H
#define	_ESTABLISHED_H

#include "Connection.h"

class Established : public TCPState {
public:
    Established();
    ~Established();

    virtual void close(Connection * c);
    virtual void receive(Connection * c, Packet * p);
};

#endif	/* _ESTABLISHED_H */

