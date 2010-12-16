/* 
 * File:   SynReceived.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:39 AM
 */

#ifndef _SYNRECEIVED_H
#define	_SYNRECEIVED_H

#include "Connection.h"


class SynReceived : public TCPState {
public:
    SynReceived();
    ~SynReceived();

    virtual void receive(Connection * c, Packet * p);
    virtual void close(Connection * c);
};

#endif	/* _SYNRECEIVED_H */

