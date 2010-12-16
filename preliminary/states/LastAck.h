/* 
 * File:   LastACK.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:42 AM
 */

#ifndef _LASTACK_H
#define	_LASTACK_H

#include "Connection.h"


class LastAck : public TCPState {
public:
    LastAck();
    ~LastAck();

    virtual void receive(Connection * c, Packet * p);
};

#endif	/* _LASTACK_H */

