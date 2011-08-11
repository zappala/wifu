/* 
 * File:   GetSockOptResponseEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 10:54 AM
 */

#ifndef GETSOCKOPTRESPONSEEVENT_H
#define	GETSOCKOPTRESPONSEEVENT_H

#include "ResponseEvent.h"

class GetSockOptResponseEvent : public ResponseEvent {
public:
    GetSockOptResponseEvent();
    virtual ~GetSockOptResponseEvent();

    void set_optval(void* optval, socklen_t optlen);
};

#endif	/* GETSOCKOPTRESPONSEEVENT_H */

