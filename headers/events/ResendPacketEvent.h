/* 
 * File:   ResendPacketEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:28 AM
 */

#ifndef _RESENDPACKETEVENT_H
#define	_RESENDPACKETEVENT_H

#include "events/PacketEvent.h"
#include "IModule.h"
#include "PacketEvent.h"

class ResendPacketEvent: public PacketEvent {
public:
    ResendPacketEvent(Socket*, WiFuPacket*);

    void execute(IModule* m);

};

#endif	/* _RESENDPACKETEVENT_H */
