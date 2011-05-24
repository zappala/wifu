/* 
 * File:   TCPTahoeCongestionControlContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOECONGESTIONCONTROLCONTEXT_H
#define	TCPTAHOECONGESTIONCONTROLCONTEXT_H

#include "OutstandingDataContext.h"
//#include "states/SlowStart.h"

// TODO: replace this with an actual state machine
#include "states/DummyCongestionController.h"

class TCPTahoeCongestionControlContext : public OutstandingDataContext {
public:
    TCPTahoeCongestionControlContext(u_int32_t iss);
    virtual ~TCPTahoeCongestionControlContext();

    u_int32_t get_snd_wnd() const;
    void set_snd_wnd(u_int32_t snd_wnd);

    u_int32_t get_snd_wnd1() const;
    void set_snd_wnd1(u_int32_t snd_wnd1);

    u_int32_t get_snd_wnd2() const;
    void set_snd_wnd2(u_int32_t snd_wnd2);
    
private:
    u_int32_t snd_wnd_;
    u_int32_t snd_wnd1_;
    u_int32_t snd_wnd2_;

};

#endif	/* TCPTAHOECONGESTIONCONTROLCONTEXT_H */

