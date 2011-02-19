/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

/**
 * Event which represents a connect event.
 * @see Event
 */
class ConnectEvent : public LibraryEvent {
public:
    /**
     * Constructs a ConnectEvent.
     *
     */
    ConnectEvent(string&, string&, Socket*);

    virtual ~ConnectEvent();

    /**
     * Will call connect() on m.
     *
     * @param m The IModule which to call connect() on.
     * @see IModule::connect()
     */
    void execute(IModule*);

    AddressPort* get_destination();

//    void set_socket_destination();

private:
    AddressPort* destination_;

};

#endif	/* _CONNECTEVENT_H */
