/* 
 * File:   SlowStart.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:55 PM
 */

#ifndef SLOWSTART_H
#define	SLOWSTART_H

#include <string>

#include "contexts/Context.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "contexts/CongestionControlContext.h"
#include "events/UDPSendPacketEvent.h"
#include "events/ConnectEvent.h"
#include "../Dispatcher.h"
using namespace std;

class SlowStart : public State {
public:
    SlowStart();
    virtual ~SlowStart();

    void send(Context* c, Socket* s, WiFuPacket* p);
    void receive(Context* c, Socket* s, WiFuPacket* p);
    void enter(Context* c);
    void exit(Context* c);
};


#endif	/* SLOWSTART_H */

