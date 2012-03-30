/* 
 * File:   SendRateLimiter.h
 * Author: leer
 *
 * Created on April 21, 2011, 3:36 PM
 */

#ifndef _SENDRATELIMITER_H
#define	_SENDRATELIMITER_H

#include "events/Event.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/TimerEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "packet/WiFuPacket.h"
#include "contexts/Context.h"
#include "Queue.h"

//temp, to be commented out when not debugging
#include "packet/TCPHeaderOption.h"
#include "defines.h"
#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"
//end temp

class SendRateLimiter : public State{

public:

    SendRateLimiter();
    
    ~SendRateLimiter();

    void setRate(int seconds,  long int nanoseconds);

    int getSeconds();

    long int getNanoseconds();

    void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

    void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);

protected:
    void dispatch_packet(Context* c, Socket* s, WiFuPacket* packet);
    void setTimer(Socket* s);

    virtual void protocol_specific_stuff(Context* c, Socket* s, WiFuPacket* packet);

private:
    bool canSend_;
    int sendSeconds_;
    long int sendNanos_;
    TimeoutEvent* waitingTimeout_;
    Queue<WiFuPacket*>* queue_;
};

#endif	/* _SENDRATELIMITER_H */

