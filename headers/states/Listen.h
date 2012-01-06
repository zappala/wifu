/* 
 * File:   Listen.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:42 PM
 */

#ifndef LISTEN_H
#define	LISTEN_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "Accept.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"

using namespace std;

class Listen : public State {
public:
    Listen();
    virtual ~Listen();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void state_close(Context* c, QueueProcessor<Event*>* q, CloseEvent* e);

private:
    void process_syn(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

};

#endif	/* LISTEN_H */

