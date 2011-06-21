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

enum ResendReason {
    UNKNOWN,
    TIMEOUT,
    THREE_DUPLICATE_ACKS
};

class ResendPacketEvent: public PacketHolder, public ProtocolEvent {
public:
    ResendPacketEvent(Socket* s, WiFuPacket* p, ResendReason reason = UNKNOWN);

    ResendPacketEvent(Socket* s, ResendReason reason = UNKNOWN);

    virtual ~ResendPacketEvent();

    virtual void execute(IModule* m);

    ResendReason get_reason() const;
    void set_reason(ResendReason reason);

private:
    ResendReason reason_;

};

#endif	/* _RESENDPACKETEVENT_H */
