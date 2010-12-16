/* 
 * File:   FinWait2.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:46 AM
 */

#ifndef _FINWAIT2_H
#define	_FINWAIT2_H

#include "Connection.h"


class FinWait2 : public TCPState {
public:
    FinWait2();
    ~FinWait2();

    virtual void receive(Connection * c, Packet * p);
};

#endif	/* _FINWAIT2_H */

