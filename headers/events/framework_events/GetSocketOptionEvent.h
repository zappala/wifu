/* 
 * File:   GetSocketOptionEvent.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 2:13 PM
 */

#ifndef GETSOCKETOPTIONEVENT_H
#define	GETSOCKETOPTIONEVENT_H

#include "SocketOptionEvent.h"
#include "Socket.h"

using namespace std;

/**
 * Represents a getsockopt() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 * @see SocketOptionEvent
 *
 */
class GetSocketOptionEvent : public SocketOptionEvent {
public:

    /**
     * Constructs a GetSocketOptionEvent.
     *
     * @param message Message received from the front end library containing the information about the getsockopt() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    GetSocketOptionEvent(string& message, string& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~GetSocketOptionEvent();

    /**
     * Calls IModule::imodule_library_get_socket_option() and passes this GetSocketOptionEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_get_socket_option() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_get_socket_option()
     */
    void execute(IModule* m);
};


#endif	/* GETSOCKETOPTIONEVENT_H */