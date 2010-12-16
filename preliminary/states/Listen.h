/* 
 * File:   Listen.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:38 AM
 */

#ifndef _LISTEN_H
#define	_LISTEN_H

#include "Connection.h"


class Listen : public TCPState {
public:

    Listen();
    ~Listen();

    virtual void close(Connection * c);
    virtual void send(Connection * c);
    virtual void receive(Connection * c, Packet * p);
};


#endif	/* _LISTEN_H */

