/* 
 * File:   CloseWait.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:41 PM
 */

#ifndef CLOSEWAIT_H
#define	CLOSEWAIT_H

#include "State.h"
#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "states/LastAck.h"

#include "events/ResponseEvent.h"
#include "events/CloseEvent.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

class CloseWait : public State {
public:
    CloseWait();
    virtual ~CloseWait();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_close(Context* c, CloseEvent* e);
    virtual bool state_can_send(Context* c, Socket* s);
};

#endif	/* CLOSEWAIT_H */

