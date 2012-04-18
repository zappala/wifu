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

using namespace std;

/**
 * Reason for a resend.
 */
enum ResendReason {
    UNKNOWN,
    TIMEOUT,
    THREE_DUPLICATE_ACKS
};

/**
 * Event which indicates that resending of data needs to occur.
 * This Event may or may not house the WiFuPacket to resent.
 *
 * @see Event
 * @see ProtocolEvent
 * @see PacketHolder
 * @see ResendReason
 */
class ResendPacketEvent: public PacketHolder, public ProtocolEvent {
public:

    /**
     * Constructs a ResendPacketEvent with a packet to resend.
     * 
     * @param s The Socket object to which this Event belongs and on which data needs to be resent.
     * @param p The packet to resend.
     * @param reason The reason for resending data.
     */
    ResendPacketEvent(Socket* s, WiFuPacket* p, ResendReason reason = UNKNOWN);

    /**
     * Constructs a ResendPacketEvent without a packet to resend.
     *
     * @param s The Socket object to which this Event belongs and on which data needs to be resent.
     * @param reason The reason for resending data.
     */
    ResendPacketEvent(Socket* s, ResendReason reason = UNKNOWN);

    /**
     * Destructor.
     */
    virtual ~ResendPacketEvent();

    /**
     * Calls IModule::imodule_resend() and passes this ResendPacketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_resend() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_resend()
     */
    virtual void execute(IModule* m);

    /**
     * @return The reason why a resend is necessary.
     */
    ResendReason get_reason() const;

    /**
     * Sets the reason why a resend is necessary.
     * @param reason Why a resend is necessary.
     */
    void set_reason(ResendReason reason);

private:

    /**
     * The reason why a resend is necessary.
     */
    ResendReason reason_;

};

#endif	/* _RESENDPACKETEVENT_H */
