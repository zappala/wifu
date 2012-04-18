/* 
 * File:   TCPAPIContextContainer.h
 * Author: leer
 *
 * Created on July 5, 2011, 2:49 PM
 */

#ifndef _TCPAPICONTEXTCONTAINER_H
#define	_TCPAPICONTEXTCONTAINER_H

#include "TCPTahoeIContextContainer.h"
#include "TCPAPRateLimiterContext.h"
#include "IContext.h"
#include "states/tcp-ap/TCPAPSendRateLimiter.h"

class TCPAPIContextContainer : public TCPTahoeIContextContainer {
public:
    TCPAPIContextContainer();
    virtual ~TCPAPIContextContainer();

    IContext* get_rate_limiter();
    void set_rate_limiter(IContext* c);
private:

    IContext* rate_limiter_;
};

#endif	/* _TCPAPICONTEXTCONTAINER_H */

