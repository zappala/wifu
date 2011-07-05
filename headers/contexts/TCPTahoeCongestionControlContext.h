/* 
 * File:   TCPTahoeCongestionControlContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOECONGESTIONCONTROLCONTEXT_H
#define	TCPTAHOECONGESTIONCONTROLCONTEXT_H

#include "OutstandingDataContext.h"
#include "states/SlowStart.h"

#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/TimerFiredEvent.h"

#include "defines.h"

/**
 * Keeps track of the state for TCP Tahoe's congestion control mechanism.
 *
 * @see TCPTahoe
 * @see OutstandingDataContext
 * @see TCPTahoeBaseCongestionControl
 * @see SlowStart
 * @see CongestionAvoidance
 */
class TCPTahoeCongestionControlContext : public OutstandingDataContext {
public:

    /**
     * Constructor.
     * Stores iss as the initial sequence number to use for a SYN.
     * Sets the probe timeout duration to 2 minutes (RFC 793 page 42).
     * Sets the congestion window to 65535 (this is overwritten in SlowStart).
     * Sets the slow start threshold to 65535 (RFC 2001 page 3).
     * Sets the current state to SlowStart.
     *
     * @param iss Initial send sequence number
     *
     * @see SlowStart
     * @see OutstandingDataContext
     */
    TCPTahoeCongestionControlContext(u_int32_t iss);

    /**
     * Destructor.
     */
    virtual ~TCPTahoeCongestionControlContext();

    /**
     * From RFC 793 especially page 72.
     * @return The send window size in bytes.
     */
    u_int16_t get_snd_wnd() const;

    /**
     * From RFC 793 especially page 72.
     * Saves the SND.WND variable.
     * @param snd_wnd The new value of SND.WND.
     */
    void set_snd_wnd(u_int16_t snd_wnd);

    /**
     * From RFC 793 especially page 72.
     * SND.WND1 is the last sequence number to update SND.WND.
     * @return SND.WND1.
     */
    u_int32_t get_snd_wnd1() const;

    /**
     * From RFC 793 especially page 72.
     * Saves the SND.WND1 variable (the last sequence number to update SND.WND).
     * @param snd_wnd1 The new value of SND.WND1.
     */
    void set_snd_wnd1(u_int32_t snd_wnd1);

    /**
     * From RFC 793 especially page 72.
     * SND.WND2 is the last acknowledgement number to update SND.WND.
     * @return SND.WND2.
     */
    u_int32_t get_snd_wnd2() const;

    /**
     * From RFC 793 especially page 72.
     * Saves the SND.WND2 variable (the last acknowledgement number to update SND.WND).
     * @param snd_wnd2 The new value of SND.WND2.
     */
    void set_snd_wnd2(u_int32_t snd_wnd2);

    /**
     * From RFC 793 around page 42.
     * @return The probe timer used to determine when a sender should query a zero receive window.
     * If it has not been set, NULL is returned.
     */
    TimeoutEvent* get_probe_timer() const;

    /**
     * From RFC 793 around page 42.
     * Saves e as the probe timer.
     * @param e TimeoutEvent representing the probe timer to save.
     */
    void set_probe_timer(TimeoutEvent* e);

    /**
     * From RFC 793 around page 42.
     * @return The duration (in seconds) the sending TCP should wait after receiving a zero in the receive window and before it should send a probe packet.
     */
    int get_probe_timer_duration() const;

    /**
     * From RFC 793 around page 42.
     * Sets the duration (in seconds) the sending TCP should wait after receiving a zero in the receive window and before it should send a probe packet.
     * @param duration The length of time the sending TCP should wait after receiving a zero in the receive window and before it should send a probe packet.
     */
    void set_probe_timer_duration(int duration);

    /**
     * From RFCs 2001 and 2581.
     * @return The current size (in bytes) of the congestion window.
     */
    u_int32_t get_cwnd() const;

    /**
     * From RFCs 2001 and 2581.
     * Saves the current congestion window.
     * @param cwnd The new congestion window size.
     */
    void set_cwnd(u_int32_t cwnd);

    /**
     * From RFCs 2001 and 2581.
     * @return The current value of the slow start threshold.
     */
    u_int16_t get_ssthreshold() const;

    /**
     * From RFCs 2001 and 2581.
     * Saves the current slow start threshold.
     * @param ssthreashold The new value of the slow start threshold.
     */
    void set_ssthreshold(u_int16_t ssthreashold);

    /**
     * Computes and returns the maximum number of bytes that the sending TCP is allowed to currently send.
     * This is the minumum of SND.WND and CWND.
     * @return The maximum number of bytes that the sending TCP is allowed to currently send.
     */
    u_int32_t get_max_allowed_to_send() const;

    /**
     * @return The maximum segment size.
     */
    u_int16_t get_mss() const;

    /**
     * Saves the maximum segment size.
     * @param mss The new value of the maximum segment size.
     */
    void set_mss(u_int16_t mss);

    /**
     * @return True if any non-control bit data has been sent.
     */
    bool is_data_sent() const;

    /**
     * Saves whether data has been sent or not.
     * @param data_sent The new value to save.
     * It should be false iff no data has been sent by this TCP.
     * It should always be true if this TCP has sent any non-control bit data.
     */
    void set_data_sent(bool data_sent);


private:

    /**
     * The last received receive window (in bytes) whose sequence and ack numbers have meet certain criteria.
     * For more information see RFC 793, especially page 72.
     * See RFC 793.
     *
     */
    u_int16_t snd_wnd_;

    /**
     * From RFC 793 page 72:
     * SND.WL1 records the sequence number of the last segment used to update SND.WND.
     */
    u_int32_t snd_wnd1_;

    /**
     * From RFC 793 page 72:
     * SND.WL2 records the acknowledgment number of the last segment used to update SND.WND.
     */
    u_int32_t snd_wnd2_;

    /**
     * The congestion window parameter.
     * It limits the amount of data that a TCP can send.
     * For more information see RFCs 2001 and 2581.
     */
    u_int32_t cwnd_;

    /**
     * The slow start threshold.
     * For more information see RFCs 2001 and 2581.
     */
    u_int16_t ssthreshold_;

    /**
     * Maximum segment size.
     * Defined as the maximum amount of data that can be sent in one packet.
     * Usually this means that MSS = MTU - header sizes.
     */
    u_int16_t mss_;

    /**
     * Timer which keeps track of when we should send a probe packet AFTER being notified that the receiver's receive window is 0.
     * For more information see RFC 793 around page 42.
     */
    TimeoutEvent* probe_timer_;

    /**
     * Duration we wait after receving a zero sized receive window before sending a probe packet
     * According to RFC 793 it should be initially set to 2 minutes.
     * We store it here as seconds.
     */
    int probe_timer_duration_;

    /**
     * Indicates whether we have sent any data bytes (excluding control bits)
     * This will be used to determine whether to update cwnd or not
     */
    bool data_sent_;
};

#endif	/* TCPTAHOECONGESTIONCONTROLCONTEXT_H */

