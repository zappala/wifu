/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "states/SimpleTCPCongestionControl.h"
#include "AddressPort.h"
#include "Socket.h"
#include "packet/WiFuPacket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"

#include "IQueue.h"
#include "Queue.h"

using namespace std;

class CongestionControlContext : public Context {
public:
    CongestionControlContext();
    virtual ~CongestionControlContext();

    int get_num_outstanding();
    void set_num_outstanding(int outstanding);

    u_int32_t get_last_sent_sequence_number();
    void set_last_sent_sequence_number(u_int32_t last_sent_seq);


private:
    int outstanding_;
    u_int32_t last_sent_sequence_number_;

};

#endif	/* CONGESTIONCONTROLCONTEXT_H */
