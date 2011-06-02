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

class SendRateLimiter : public State{

public:

    SendRateLimiter();
    
    ~SendRateLimiter();

    void setRate(int seconds,  long int nanoseconds);

    int getSeconds();

    long int getNanoseconds();

    void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

    void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);
    
private:
    bool canSend_;
    int sendSeconds_;
    long int sendNanos_;
    TimeoutEvent* waitingTimeout_;
    Queue<WiFuPacket*>* queue_;

    void setTimer(Socket* s);
};

#endif	/* _SENDRATELIMITER_H */

