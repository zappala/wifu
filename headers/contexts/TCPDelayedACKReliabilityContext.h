/* 
 * File:   TCPDelayedACKReliabilityContext.h
 * Author: leer
 *
 * Created on June 17, 2011, 2:09 PM
 */

#ifndef _TCPDELAYEDACKRELIABILITYCONTEXT_H
#define	_TCPDELAYEDACKRELIABILITYCONTEXT_H

#include "contexts/TCPTahoeReliabilityContext.h"
#include "states/tcp-ap/TCPDelayedACKReliabilityState.h"

class TCPDelayedACKReliabilityContext : public TCPTahoeReliabilityContext {
public:
    TCPDelayedACKReliabilityContext(u_int32_t iss);
    virtual ~TCPDelayedACKReliabilityContext();

        TimeoutEvent* get_ack_timeout_event();
    void set_ack_timeout_event(TimeoutEvent* e);

    u_int16_t get_delay_count();
    void set_delay_count(u_int16_t count);

    double get_delay_timeout_interval();
    void set_delay_timeout_interval(double count);

    u_int32_t get_l1_threshold();
    void set_l1_threshold(u_int32_t l1);

    u_int32_t get_l2_threshold();
    void set_l2_threshold(u_int32_t l2);

    u_int32_t get_l3_threshold();
    void set_l3_threshold(u_int32_t l3);

    u_int16_t get_delay1();
    void set_delay1(u_int16_t delay);

    u_int16_t get_delay2();
    void set_delay2(u_int16_t delay);

    u_int16_t get_delay3();
    void set_delay3(u_int16_t delay);

    u_int16_t get_delay4();
    void set_delay4(u_int16_t delay);
    
private:

    u_int16_t delay_count_;

    double delay_timeout_interval_;

    TimeoutEvent* ack_timer_;

    u_int32_t l1_threshold_;
    u_int32_t l2_threshold_;
    u_int32_t l3_threshold_;
    u_int16_t delay1_;
    u_int16_t delay2_;
    u_int16_t delay3_;
    u_int16_t delay4_;
};

#endif	/* _TCPDELAYEDACKRELIABILITYCONTEXT_H */

