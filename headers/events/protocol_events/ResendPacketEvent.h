/* 
 * File:   ResendPacketEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:28 AM
 */

#ifndef _RESENDPACKETEVENT_H
#define	_RESENDPACKETEVENT_H

#include "IModule.h"
#include "PacketHolder.h"
#include "ProtocolEvent.h"

class ResendPacketEvent: public PacketHolder, public ProtocolEvent {
public:
    ResendPacketEvent(Socket* s, WiFuPacket* p);

    void execute(IModule* m);

};

#endif	/* _RESENDPACKETEVENT_H */
