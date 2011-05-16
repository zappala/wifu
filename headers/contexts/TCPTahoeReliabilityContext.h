/* 
 * File:   TCPTahoeReliabilityContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOERELIABILITYCONTEXT_H
#define	TCPTAHOERELIABILITYCONTEXT_H

#include <sys/types.h>

#include "Context.h"
#include "states/TCPTahoeReliabilityState.h"

#include "events/TimeoutEvent.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"


class TCPTahoeReliabilityContext : public Context {
public:
    TCPTahoeReliabilityContext();
    virtual ~TCPTahoeReliabilityContext();

    u_int32_t get_snd_una();
    void set_snd_una(u_int32_t snd_una);

    u_int32_t get_snd_nxt();
    void set_snd_nxt(u_int32_t snd_nxt);

    u_int32_t get_rcv_nxt();
    void set_rcv_nxt(u_int32_t rcv_nxt);

    u_int16_t get_rcv_wnd();
    void set_rcv_wnd(u_int16_t rcv_wnd);

    TimeoutEvent* get_timeout_event();
    void set_timeout_event(TimeoutEvent* e);

    int get_rto();
    void set_rto(int rto);

    u_int32_t get_duplicate_ack_number();
    void set_duplicate_ack_number(u_int32_t num);

    int get_duplicates();
    void set_duplicates(int duplicates);

    vector<TCPPacket*>& get_packet_buffer();
    
private:
    u_int32_t snd_una_;
    u_int32_t snd_nxt_;

    u_int32_t rcv_nxt_;
    u_int16_t rcv_wnd_;

    TimeoutEvent* timer_;
    /**
     * Time in seconds for the next timeout
     */
    int rto_;

    u_int32_t duplicate_ack_number_;
    int duplicates_;

    vector<TCPPacket*> packet_buffer_;
};

#endif	/* TCPTAHOERELIABILITYCONTEXT_H */

