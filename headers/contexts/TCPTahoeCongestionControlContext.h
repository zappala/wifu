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

    u_int16_t get_cwnd() const;
    void set_cwnd(u_int16_t cwnd);

    u_int16_t get_ssthreshold() const;
    void set_ssthreshold(u_int16_t ssthreashold);

    u_int16_t get_max_allowed_to_send() const;

    u_int16_t get_mss() const;
    void set_mss(u_int16_t mss);
private:

    u_int16_t snd_wnd_;
    u_int32_t snd_wnd1_;
    u_int32_t snd_wnd2_;

    u_int16_t cwnd_;
    u_int16_t ssthreashold_;
    u_int16_t mss_;

    TimeoutEvent* probe_timer_;

    /**
     * Duration which the probe timer lasts
     * According to RFC 793 it should be initially set to 2 minutes
     * We store it here as seconds
     */
    int probe_timer_duration_;

};

#endif	/* TCPTAHOECONGESTIONCONTROLCONTEXT_H */

