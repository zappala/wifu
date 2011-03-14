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
#include "states/SlowStart.h"
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

    bool can_send_data();
    void set_can_send_data(bool can_send);

private:
    bool can_send_;

};

#endif	/* CONGESTIONCONTROLCONTEXT_H */
