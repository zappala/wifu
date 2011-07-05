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
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/TimerFiredEvent.h"

#include "IQueue.h"
#include "Queue.h"

using namespace std;

/**
 * Context of SimpleTCP's congestion control FSM.
 */
class SimpleTCPCongestionControlContext : public Context {
public:
    
    /**
     * Contstructor.
     * Initializes current state to SimpleTCPCongestionControl.
     */
    SimpleTCPCongestionControlContext();

    /**
     * Destructor.
     */
    virtual ~SimpleTCPCongestionControlContext();

    /**
     * @return The number of packets (not bytes) that have not yet been acknowledged.
     */
    int get_num_outstanding();

    /**
     * Saves the number of packets (not bytes) that have not yet been acknowledged.
     * @param outstanding The value to save.
     */
    void set_num_outstanding(int outstanding);

    /**
     * @return The last sent sequence number marking the packet (not byte(s)) last sent.
     */
    u_int32_t get_last_sent_sequence_number();

    /**
     * Saves the last sent packet's sequence number.
     * @param last_sent_seq The value to save.
     */
    void set_last_sent_sequence_number(u_int32_t last_sent_seq);


private:
    /**
     * The number of packets that are outstanding.
     */
    int outstanding_;

    /**
     * The last sent sequence number marking the packet last sent.
     */
    u_int32_t last_sent_sequence_number_;

};

#endif	/* CONGESTIONCONTROLCONTEXT_H */
