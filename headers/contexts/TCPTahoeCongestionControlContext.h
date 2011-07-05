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

class TCPTahoeCongestionControlContext : public OutstandingDataContext {
public:
    TCPTahoeCongestionControlContext(u_int32_t iss);
    virtual ~TCPTahoeCongestionControlContext();

    u_int16_t get_snd_wnd() const;
    void set_snd_wnd(u_int16_t snd_wnd);

    u_int32_t get_snd_wnd1() const;
    void set_snd_wnd1(u_int32_t snd_wnd1);

    u_int32_t get_snd_wnd2() const;
    void set_snd_wnd2(u_int32_t snd_wnd2);

    TimeoutEvent* get_probe_timer() const;
    void set_probe_timer(TimeoutEvent* e);

    int get_probe_timer_duration() const;
    void set_probe_timer_duration(int duration);

    u_int32_t get_cwnd() const;
    void set_cwnd(u_int32_t cwnd);

    u_int16_t get_ssthreshold() const;
    void set_ssthreshold(u_int16_t ssthreashold);

    u_int32_t get_max_allowed_to_send() const;

    u_int16_t get_mss() const;
    void set_mss(u_int16_t mss);

    bool is_data_sent() const;
    void set_data_sent(bool data_sent);


private:

    /**
     * The last received receive window whose sequence and ack numbers have meet certain criteria.
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
     * For more information see RFC 793 around page 43.
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

