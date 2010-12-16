/* 
 * File:   Closed.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:31 AM
 */

#ifndef _CLOSED_H
#define	_CLOSED_H

#include "Connection.h"

class Closed : public TCPState {
public:
    Closed();
    ~Closed();

    virtual void passiveOpen(Connection * c);
    virtual void activeOpen(Connection * c);

};


#endif	/* _CLOSED_H */

