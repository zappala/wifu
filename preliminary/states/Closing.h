/* 
 * File:   Closing.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 9:44 AM
 */

#ifndef _CLOSING_H
#define	_CLOSING_H

#include "Connection.h"


class Closing : public TCPState {
public:
    Closing();
    ~Closing();

    virtual void receive(Connection * c, Packet * p);
};


#endif	/* _CLOSING_H */

