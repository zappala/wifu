/* 
 * File:   TCPAPSendRateLimiter.h
 * Author: leer
 *
 * Created on July 1, 2011, 5:05 PM
 */

#ifndef _TCPAPSENDRATELIMITER_H
#define	_TCPAPSENDRATELIMITER_H

#include "states/tcp-ap/SendRateLimiter.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "contexts/TCPAPRateLimiterContext.h"
#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"
#include "../../Math.h"

#include <fstream>
#include <iostream>
#include <map>
#include <list>

class TCPAPSendRateLimiter : public SendRateLimiter {
public:
    TCPAPSendRateLimiter();
    virtual ~TCPAPSendRateLimiter();

    void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    
    void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    void state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

protected:
    virtual void protocol_specific_stuff(Context* c, Socket* s, WiFuPacket* packet);

private:

    double calc_covariance(Context* c);
};

#endif	/* _TCPAPSENDRATELIMITER_H */

