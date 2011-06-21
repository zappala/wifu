/* 
 * File:   TCPDelayedACKReliabilityState.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:25 PM
 */

#ifndef TCPDELAYEDACKRELIABILITYSTATE_H
#define	TCPDELAYEDACKRELIABILITYSTATE_H

#include "states/State.h"

#include "events/framework_events/TimerFiredEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/ResponseEvent.h"

#include "contexts/TCPDelayedACKReliabilityContext.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

#include "Math.h"

class TCPDelayedACKReliabilityState : public State {
public:
    TCPDelayedACKReliabilityState();
    virtual ~TCPDelayedACKReliabilityState();

    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);
    virtual void state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);

private:
    void start_timer(Context* c, Socket* s);
    void reset_timer(Context* c, Socket* s);
    void cancel_timer(Context* c, Socket* s);
    void start_ack_timer(Context* c, Socket* s);
    //void reset_ack_timer(Context* c, Socket* s);
    void cancel_ack_timer(Context* c, Socket* s);
    void update_rto(Context* c, TCPTimestampOption* ts);
    void resend_data(Context* c, QueueProcessor<Event*>* q, Socket* s);
    void create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);    
    void create_and_dispatch_ack(QueueProcessor<Event*>* q, Socket* s);

    // <editor-fold defaultstate="collapsed" desc="state_send_packet() helper functions">
    void set_sequence_number_and_update_window_variables(Context* c, TCPPacket* p);
    void set_ack_number_and_bit(Context* c, TCPPacket* p);
    void check_and_start_timer(Context*c, SendPacketEvent* e);
    void append_control_bytes_to_send_buffer(SendPacketEvent* e);
    void insert_timestamp(Context* c, TCPPacket* p);
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="state_receive_packet() helper functions">
    void update_echo_reply(Context* c, TCPPacket* p);

    /**
     * Handles the ack portion of the incoming packet
     * @param c The Context
     * @param q The QueueProcessor which e came from
     * @param e The Event received from the network interface
     * @return True if we may continue (and try to process data), false if it is an invalid ack
     */
    bool handle_ack(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void handle_valid_ack(Context* c, NetworkReceivePacketEvent* e);
    void handle_duplicate_ack(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void handle_control_bits_and_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void handle_control_bits(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    // </editor-fold>

    u_int16_t cur_ack_count_;
    u_int32_t ack_timeout_delay;
    
};

#endif	/* TCPDelayedACKReliabilityState_H */

