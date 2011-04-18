/* 
 * File:   WifuEndBackEndLibrary.h
 * Author: rbuck
 *
 * Created on November 26, 2010, 2:19 PM
 */

#ifndef WIFUENDBACKENDLIBRARY_H
#define	WIFUENDBACKENDLIBRARY_H

#include "LocalSocketFullDuplex.h"
#include "Module.h"
#include "QueryStringParser.h"
#include "Socket.h"
#include "SocketCollection.h"
#include "ProtocolManager.h"
#include "Utils.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "events/SocketEvent.h"
#include "events/BindEvent.h"
#include "events/ResponseEvent.h"
#include "events/ListenEvent.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/ReceiveEvent.h"
#include "events/SendEvent.h"
#include "events/CloseEvent.h"
#include "PortManager.h"

/**
 * Translates string messages received from the front-end library into Event objects
 * and enqueues them to the dispatcher (and visa-versa).
 */
class WifuEndBackEndLibrary : public LocalSocketFullDuplex, public Module {
public:
    static WifuEndBackEndLibrary& instance();

    virtual ~WifuEndBackEndLibrary();

    /**
     * Callback function when the Unix socket receives a message
     * Will convert the message to an Event and queue it onto the Dispatcher
     * The guts of this method are copied from test/WifuEndAPItest.h
     *
     * @param message Message received from the front-end API
     */
    void receive(string& message);

    void library_response(Event* e);

private:
    WifuEndBackEndLibrary();

};

#endif	/* WIFUENDBACKENDLIBRARY_H */

