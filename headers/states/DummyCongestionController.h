/* 
 * File:   DummyCongestionController.h
 * Author: rbuck
 *
 * Created on May 23, 2011, 2:59 PM
 */

#ifndef DUMMYCONGESTIONCONTROLLER_H
#define	DUMMYCONGESTIONCONTROLLER_H

#include "State.h"

#include "contexts/Context.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "Math.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

class DummyCongestionController : public State {
public:
    DummyCongestionController();
    DummyCongestionController(const DummyCongestionController& orig);
    virtual ~DummyCongestionController();
    
    void state_send_packet(Context* c, SendPacketEvent* e);
    void state_resend_packet(Context* c, ResendPacketEvent* e);
    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e);
private:
    
    void send_packets(Context* c, Event* e);

};

#endif	/* DUMMYCONGESTIONCONTROLLER_H */

