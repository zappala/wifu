/* 
 * File:   WiFuTransportBackEndTranslator.h
 * Author: rbuck
 *
 * Created on November 26, 2010, 2:19 PM
 */

#ifndef WIFUTRANSPORTBACKENDTRANSLATOR_H
#define	WIFUTRANSPORTBACKENDTRANSLATOR_H


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
#include "ObjectPool.h"
#include "Logger.h"
#include "MessageStructDefinitions.h"

#include <map>

/**
 * Translates string messages received from the front-end library into Event objects
 * and enqueues them to the dispatcher (and visa-versa).
 */
class WiFuTransportBackEndTranslator : public LocalSocketFullDuplex, public Module {
public:
    static WiFuTransportBackEndTranslator& instance();

    virtual ~WiFuTransportBackEndTranslator();

    /**
     * Callback function when the Unix socket receives a message
     * Will convert the message to an Event and queue it onto the Dispatcher
     *
     * @param message Message received from the front-end API
     */
    void receive(unsigned char* message, int length);

    void imodule_library_response(Event* e);

private:
    WiFuTransportBackEndTranslator();

    map<int, LibraryEvent*, less<int>, gc_allocator<pair<int, LibraryEvent*> > > event_map_;
    map<int, LibraryEvent*, less<int>, gc_allocator<pair<int, LibraryEvent*> > >::iterator event_map_iterator_;
};

#endif	/* WIFUTRANSPORTBACKENDTRANSLATOR_H */

