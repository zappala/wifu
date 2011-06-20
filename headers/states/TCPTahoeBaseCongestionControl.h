/* 
 * File:   TCPTahoeBaseCongestionControl.h
 * Author: rbuck
 *
 * Created on May 23, 2011, 2:59 PM
 */

#ifndef TCPTahoeBaseCongestionControl_H
#define	TCPTahoeBaseCongestionControl_H

#include "State.h"

#include "contexts/Context.h"

#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"

#include "Math.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

class TCPTahoeBaseCongestionControl : public State {
public:
    TCPTahoeBaseCongestionControl();
    TCPTahoeBaseCongestionControl(const TCPTahoeBaseCongestionControl& orig);
    virtual ~TCPTahoeBaseCongestionControl();

    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);
    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    virtual void set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);
private:

    void resend_data(Context* c, QueueProcessor<Event*>* q, Event* e);
    void send_one_packet(Context* c, QueueProcessor<Event*>* q, Event* e, bool ignore_window = false);
    void send_packets(Context* c, QueueProcessor<Event*>* q, Event* e);
    int get_send_data_length(Context* c, Event* e, WiFuPacket* p, bool ignore_window);
    int get_resend_data_length(Context* c, Event* e, WiFuPacket* p);



};

#endif	/* TCPTahoeBaseCongestionControl_H */

