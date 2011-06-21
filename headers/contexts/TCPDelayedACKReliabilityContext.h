/* 
 * File:   TCPDelayedACKReliabilityContext.h
 * Author: leer
 *
 * Created on June 17, 2011, 2:09 PM
 */

#ifndef _TCPDELAYEDACKRELIABILITYCONTEXT_H
#define	_TCPDELAYEDACKRELIABILITYCONTEXT_H

#include <sys/types.h>

#include "OutstandingDataContext.h"
#include "states/tcp-ap/TCPDelayedACKReliabilityState.h"

#include "events/framework_events/TimeoutEvent.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"
#include "TCPPacketBuffer.h"
#include "defines.h"


class TCPDelayedACKReliabilityContext : public OutstandingDataContext {
public:
    TCPDelayedACKReliabilityContext(u_int32_t iss);
    virtual ~TCPDelayedACKReliabilityContext();

    bool is_initialized();
    void set_initialized();

    u_int32_t get_rcv_nxt();
    void set_rcv_nxt(u_int32_t rcv_nxt);

    u_int16_t get_rcv_wnd();
    void set_rcv_wnd(u_int16_t rcv_wnd);

    TimeoutEvent* get_timeout_event();
    void set_timeout_event(TimeoutEvent* e);

    TimeoutEvent* get_ack_timeout_event();
    void set_ack_timeout_event(TimeoutEvent* e);

    double get_rto();
    void set_rto(double rto);

    u_int32_t get_duplicate_ack_number();
    void set_duplicate_ack_number(u_int32_t num);

    int get_duplicates();
    void set_duplicates(int duplicates);

    TCPPacketBuffer& get_receive_window();

    ReceiveEvent* get_receive_event();
    void set_receive_event(ReceiveEvent* e);

    u_int32_t get_echo_reply();
    void set_echo_reply(u_int32_t echo_reply);

    double get_srtt();
    void set_srtt(double srtt);

    double get_rttvar();
    void set_rttvar(double rttvar);

    u_int16_t get_delay_count();
    void set_delay_count(u_int16_t count);

    double get_delay_timeout_interval();
    void set_delay_timeout_interval(double count);

private:

    bool initialized_;

    u_int32_t snd_una_;
    u_int32_t snd_nxt_;

    u_int32_t rcv_nxt_;
    u_int16_t rcv_wnd_;

    u_int16_t delay_count_;

    double delay_timeout_interval_;

    TimeoutEvent* timer_;
    TimeoutEvent* ack_timer_;
    ReceiveEvent* receive_event_;
    /**
     * Time in seconds for the next timeout
     */
    double rto_;

    u_int32_t duplicate_ack_number_;
    int duplicates_;

    TCPPacketBuffer receive_window_;

    // Timer variables
    // in microseconds
    u_int32_t echo_reply_;

    // in seconds
    double srtt_;
    double rttvar_;

};

#endif	/* _TCPDELAYEDACKRELIABILITYCONTEXT_H */

