/* 
 * File:   TimeWait.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:47 AM
 */

#ifndef _TIMEWAIT_H
#define	_TIMEWAIT_H

#include "Connection.h"


class TimeWait : public TCPState {
public:
    TimeWait();
    ~TimeWait();

    virtual void timeout(Connection * c);
};

#endif	/* _TIMEWAIT_H */

