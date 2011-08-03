/* 
 * File:   TCPTahoeReliabilityContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOERELIABILITYCONTEXT_H
#define	TCPTAHOERELIABILITYCONTEXT_H

#include <sys/types.h>

#include "OutstandingDataContext.h"
#include "states/TCPTahoeReliabilityState.h"

#include "events/framework_events/TimeoutEvent.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"
#include "TCPPacketBuffer.h"
#include "defines.h"

/**
 * Context of the TCP Tahoe Reliability finite state machine.
 * Holds all the state needed to perform TCP Tahoe type reliability.
 * For more information on some of these variables see RFCs 793, 2988.
 * Currently, TCP Tahoe's reliability FSM has only one state: TCPTahoeReliabilityState.
 *
 * @see TCPTahoeReliabilityState
 * @see OutstandingDataContext
 */
class TCPTahoeReliabilityContext : public OutstandingDataContext {
public:

    /**
     * Constructor.
     * Saves the initial sequence number to use when sending a SYN.
     * Sets default values for all variables.
     * Of note, it sets RCV_WND to the max allowed (65535) and RTO to 3 seconds.
     * @param iss The initial sequence number to use when sending a SYN.
     */
    TCPTahoeReliabilityContext(u_int32_t iss);

    /**
     * Destructor.
     */
    virtual ~TCPTahoeReliabilityContext();

    /**
     * @return True if TCPTahoeReliabiltyState has initialized, false otherwise.
     */
    bool is_initialized();

    /**
     * @post is_initialized() will always return true.
     */
    void set_initialized();

    /**
     * From RFC 793.
     * @return The next in order expected byte.
     */
    u_int32_t get_rcv_nxt() const;

    /**
     * From RFC 793.
     * Saves RCV_NXT.
     * @param rcv_nxt The next in order expected byte.
     */
    void set_rcv_nxt(u_int32_t rcv_nxt);

    /**
     * From RFC 793.
     * @return The number of bytes that the receiver is willing to accept.
     */
    u_int16_t get_rcv_wnd() const;

    /**
     * From RFC 793.
     * Saves RCV_WND.
     * @param rcv_wnd The number of bytes that the receiver is willing to accept.
     */
    void set_rcv_wnd(u_int16_t rcv_wnd);

    /**
     * From RFC 793.
     * We need to know if the last sent rcv_wnd was 0.
     * @return The last value of rcv_wnd_ that was sent in an ack.
     */
    u_int16_t get_last_sent_rcv_wnd() const;

    /**
     * From RFC 793.
     * We need to know if the last sent rcv_wnd was 0.
     * @param last_sent_rcv_wnd The last value of rcv_wnd_ that was sent in an ack.
     */
    void set_last_sent_rcv_wnd(u_int16_t last_sent_rcv_wnd);

    /**
     * @return The TimeoutEvent used to indicate when we need to resend due to not receiving any acknowledgements.
     */
    TimeoutEvent* get_timeout_event() const;

    /**
     * Saves e as the TimeoutEvent we will expect to get when it fires.
     * @param e The TimeoutEvent we set to indicate not receiving any acknowledgements.
     */
    void set_timeout_event(TimeoutEvent* e);

    /**
     * From RFC 2988.
     * @return The current retransmission timeout value in seconds.
     */
    double get_rto() const;

    /**
     * From RFC 2988.
     * Saves the retansmission timeout to rto seconds.
     * @param rto The value to save in seconds of the retransmission timeout.
     */
    void set_rto(double rto);

    /**
     * From RFC 2001 (fast retransmit).
     * @return The latest ack numer we have receieved.
     * This is used to compare to an incoming ack to see if we have seen it before or not.
     */
    u_int32_t get_duplicate_ack_number() const;

    /**
     * From RFC 2001 (fast retransmit).
     * Saves num as the latest ack number that we have seen.
     * @param num The latest ack number we have received.
     */
    void set_duplicate_ack_number(u_int32_t num);

    /**
     * From RFC 2001 (fast retransmit).
     * @return The number of duplicate acks we have seen.
     */
    int get_duplicates() const;

    /**
     * From RFC 2001 (fast retransmit).
     * Sets the number of duplicate acks we have seen to duplicates.
     * @param duplicates The number of duplicate acks we have seen.
     */
    void set_duplicates(int duplicates);

    /**
     * @return Reference to the TCPPacketBuffer object which buffers and orders data.
     * @see TCPPacketBuffer
     */
    TCPPacketBuffer& get_receive_window();

    /**
     * @return The ReceiveEvent representing a request from the application to receive data from the receive bufffer.
     */
    ReceiveEvent* get_receive_event() const;

    /**
     * Saves the request from the application to receive data.
     * @param e  The request from the application to receive data.
     */
    void set_receive_event(ReceiveEvent* e);

    /**
     * From RFC 1323.
     * @return The latest timestamp value we have received.
     */
    u_int32_t get_echo_reply() const;

    /**
     * From RFC 1323.
     * Saves the latest timestamp value we have received.
     * @param ts Timestamp value from the latest ack we have received.
     */
    void set_echo_reply(u_int32_t ts);

    /**
     * RFC 2988.
     * @return The smoothed RTT.
     */
    double get_srtt() const;

    /**
     * RFC 2988.
     * Saves the smoothed RTT.
     * @param srtt The smoothed RTT.
     */
    void set_srtt(double srtt);

    /**
     * RFC 2988.
     * @return The RTT variance.
     */
    double get_rttvar() const;

    /**
     * RFC 2988.
     * Saves the RTT variance.
     * @param rttvar The RTT variance.
     */
    void set_rttvar(double rttvar);

    int get_receive_index() const;

    void set_receive_index(int receive_index);

private:

    /**
     * Keeps track of whether we have initialized member variables.
     */
    bool initialized_;

    /**
     * RFC 793.
     * Sequence number of the next expected in order byte.
     */
    u_int32_t rcv_nxt_;

    /**
     * RFC 793.
     * Number of bytes that this receiver can currently buffer.
     */
    u_int16_t rcv_wnd_;

    /**
     * TimeoutEvent to compare to when a TimerFires.
     */
    TimeoutEvent* timer_;

    /**
     * ReceiveEvent that the application has sent but we cannot currently respond to.
     */
    ReceiveEvent* receive_event_;

    /**
     * RFC 2988.
     * Time in seconds for the next timeout
     */
    double rto_;

    /**
     * The latest ack number we have received to update duplicates_.
     */
    u_int32_t duplicate_ack_number_;

    /**
     * From RFC 2001 (fast retransmit).
     * The number of consecutive ack numbers received that have been identicle.
     */
    int duplicates_;

    /**
     * Buffers and puts data in order.
     */
    TCPPacketBuffer receive_window_;

    /**
     * From RFC 1323.
     * Latest timestamp value we have received.
     * It will be inserted in every outgoing packet in the echo reply field which supports the timestamp option.
     */
    u_int32_t echo_reply_;

    /**
     * RFC 2988.
     * Smoothed RTT in seconds.
     */
    double srtt_;

    /**
     * RFC 2988.
     * RTT variance in seconds.
     */
    double rttvar_;

    int receive_index_;

};

#endif	/* TCPTAHOERELIABILITYCONTEXT_H */

