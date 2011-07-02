/* 
 * File:   TCPFewCongestionControlContext.h
 * Author: leer
 *
 * Created on June 28, 2011, 2:50 PM
 */

#ifndef _TCPFEWCONGESTIONCONTROLCONTEXT_H
#define	_TCPFEWCONGESTIONCONTROLCONTEXT_H

#include "states/FewSlowStart.h"
#include "contexts/TCPTahoeCongestionControlContext.h"

#include "defines.h"
#include "Math.h"

class TCPFewCongestionControlContext : public TCPTahoeCongestionControlContext {
public:
    TCPFewCongestionControlContext(u_int32_t iss);
    virtual ~TCPFewCongestionControlContext();

//    double get_frac_cwnd() const;
//    void set_frac_cwnd(double frac_cwnd);

    double get_alpha() const;
    void set_alpha(double alpha);

    u_int32_t get_min_cwnd_size() const;
    void set_min_cwnd_size(u_int32_t minsize);
    
private:

    double frac_cwnd_;
    double alpha_;
    double min_cwnd_size_;
};

#endif	/* _TCPFEWCONGESTIONCONTROLCONTEXT_H */

