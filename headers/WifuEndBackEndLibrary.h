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
#include "events/framework_events/SocketEvent.h"
#include "events/framework_events/BindEvent.h"
#include "events/framework_events/ResponseEvent.h"
#include "events/framework_events/ListenEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/ReceiveEvent.h"
#include "events/framework_events/SendEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"
#include "PortManager.h"

#include "Logger.h"

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
    void receive(gcstring& message, u_int64_t& receive_time);

    void receive(unsigned char* message, int length, u_int64_t& receive_time);

    void imodule_library_response(Event* e);

private:
    WifuEndBackEndLibrary();

    list<u_int64_t, gc_allocator<u_int64_t> > receive_events_, recv_response_events_;
    list<gcstring, gc_allocator<gcstring> > recv_response_sizes_;

    list<u_int64_t, gc_allocator<u_int64_t> > send_events_, send_response_events_;
    list<gcstring, gc_allocator<gcstring> > send_response_sizes_;

};

#endif	/* WIFUENDBACKENDLIBRARY_H */

