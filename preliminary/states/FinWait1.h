/* 
 * File:   FinWait1.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:43 AM
 */

#ifndef _FINWAIT1_H
#define	_FINWAIT1_H

#include "Connection.h"


class FinWait1 : public TCPState {
public:
    FinWait1();
    ~FinWait1();

    virtual void receive(Connection * c, Packet * p);
};


#endif	/* _FINWAIT1_H */

