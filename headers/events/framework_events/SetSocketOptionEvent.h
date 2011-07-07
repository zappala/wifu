/* 
 * File:   SetSocketOptionEvent.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 1:40 PM
 */

#ifndef SETSOCKETOPTIONEVENT_H
#define	SETSOCKETOPTIONEVENT_H

#include "SocketOptionEvent.h"
#include "Socket.h"

using namespace std;

/**
 * Represents a setsockopt() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 * @see SocketOptionEvent
 *
 */
class SetSocketOptionEvent : public SocketOptionEvent {
public:

    /**
     * Constructs a SetSocketOptionEvent.
     *
     * @param message Message received from the front end library containing the information about the setsockopt() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    SetSocketOptionEvent(gcstring& message, gcstring& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~SetSocketOptionEvent();

    /**
     * Calls IModule::imodule_library_set_socket_option() and passes this SetSocketOptionEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_set_socket_option() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_set_socket_option()
     */
    void execute(IModule* m);

    /**
     * @return The value to set the option to.
     */
    gcstring& get_option_value();

    /**
     * Helper function to create the value pair used in the SocketOptions map.
     * @return A pair of value and value-length for this socket option event.
     * @see SocketOptions
     */
    pair<gcstring, socklen_t> get_value_length_pair();
};

#endif	/* SETSOCKETOPTIONEVENT_H */