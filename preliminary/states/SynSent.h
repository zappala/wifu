/* 
 * File:   SynSent.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:40 AM
 */

#ifndef _SYNSENT_H
#define	_SYNSENT_H

#include "Connection.h"


class SynSent : public TCPState {
public:
    SynSent();
    ~SynSent();

    virtual void close(Connection * c);
    virtual void receive(Connection * c, Packet * p);
};


#endif	/* _SYNSENT_H */

