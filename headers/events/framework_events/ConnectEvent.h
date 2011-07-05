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
 * Represents a connect() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class ConnectEvent : public LibraryEvent {
public:

    /**
     * Constructs a ConnectEvent.
     *
     * @param message Message received from the front end library containing the information about the connect() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    ConnectEvent(string& message, string& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~ConnectEvent();

    /**
     * Calls IModule::imodule_library_connect() and passes this CloseEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_connect() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_connect()
     */
    void execute(IModule* m);

    /**
     * The first time this function is called the source address and port will need to be
     * retrieved from the internal map and used in creation of an AddressPort object.
     * The pointer to this created object is returned each time (only one AddressPort object is created).
     *
     * This is the destination host to connect to.
     *
     * @return The destination address and port passed into the connect() socket call and which will identify the host to connect to.
     */
    AddressPort* get_destination();

private:

    /**
     * Pointer so we can lazily-evaluate and save the AddressPort when we call ConnectEvent::get_destination().
     * Holds the remote address and port to connect to.
     */
    AddressPort* destination_;

};

#endif	/* _CONNECTEVENT_H */
