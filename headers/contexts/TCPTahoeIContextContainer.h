/* 
 * File:   TCPTahoeIContextContainer.h
 * Author: rbuck
 *
 * Created on May 4, 2011, 3:55 PM
 */

#ifndef TCPTAHOEICONTEXTCONTAINER_H
#define	TCPTAHOEICONTEXTCONTAINER_H

#include "contexts/TCPTahoeCongestionControlContext.h"
#include "contexts/TCPTahoeReliabilityContext.h"
#include "contexts/ConnectionManagerContext.h"

#include "events/framework_events/SendEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"

#include "BasicIContextContainer.h"

/**
 * Keeps track of the state for each connection.
 * It contains three FSM: connection management, reliability, and congestion control.
 * Also houses additional protocol information to help glue the FSM together.
 */
class TCPTahoeIContextContainer : public BasicIContextContainer {
public:

    /**
     * Constructor.
     * Sets the initial send sequence number to 1.
     * Creates new contexts for each of the three FSM.
     */
    TCPTahoeIContextContainer();

    /**
     * Destructor.
     */
    ~TCPTahoeIContextContainer();

    /**
     * @return The saved SendEvent if any, NULL otherwise.
     */
    SendEvent* get_saved_send_event();

    /**
     * Saves e.
     * e should be non-null iff the send buffer is full and must wait to put data into it.
     * @param e The SendEvent representing an application call to send() or sendto() whose data cannot be put into the send buffer because it is full.
     */
    void set_saved_send_event(SendEvent* e);

    /**
     * @return The saved CloseEvent if any, NULL otherwise.
     */
    CloseEvent* get_saved_close_event();

    /**
     * Saves e.
     * e should only be saved iff the application calls close() and the send buffer is empty.
     * @param e The CloseEvent representing an application close().
     */
    void set_saved_close_event(CloseEvent* e);

    /**
     * @return The saved packet containing a FIN if any, NULL otherwise.
     */
    NetworkReceivePacketEvent* get_saved_fin();

    /**
     * Saves e.
     * e should only be saved iff we have received a FIN (contained in e) and the receive window is less than its starting value.
     * @param e The NetworkReceivePacketEvent containing a FIN.
     */
    void set_saved_fin(NetworkReceivePacketEvent* e);

private:
    /**
     * When an application calls send() or sendto(), a SendEvent is generated on the back-end.
     * If the send buffer is full, send() and sendto() block.
     * To mimic the block, we must save the SendEvent without returning a response to the front end.
     */
    SendEvent* saved_send_event_;

    /**
     * When an application calls close(), a CloseEvent is generated on the back-end.
     * If the send buffer is not empty we must hang onto this event as it initiates a close.
     */
    CloseEvent* saved_close_event_;

    /**
     * When we receive a FIN we know the other host is done sending data.
     * However, we do not want to process the received FIN until the receive buffer is empty so we don't notify the application that we should close before we have delivered all data to the application.
     */
    NetworkReceivePacketEvent* fin_;
};

#endif	/* TCPTAHOEICONTEXTCONTAINER_H */

