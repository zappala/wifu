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
#include "events/SendBufferNotEmptyEvent.h"
#include "events/ReceiveEvent.h"

class DummyCongestionController : public State {
public:
    DummyCongestionController();
    DummyCongestionController(const DummyCongestionController& orig);
    virtual ~DummyCongestionController();
    
    void state_receive(Context* c, ReceiveEvent* e);
    void state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e);
private:
    
    void send_packets(Context* c, Event* e);

};

#endif	/* DUMMYCONGESTIONCONTROLLER_H */

