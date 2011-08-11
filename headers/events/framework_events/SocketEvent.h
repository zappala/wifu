/* 
 * File:   SocketEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:55 PM
 */

#ifndef SOCKETEVENT_H
#define	SOCKETEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

using namespace std;

/**
 * Represents a socket() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class SocketEvent : public LibraryEvent {
public:

    /**
     * Constructs a SocketEvent.
     *
     * @param m Map received from the front end library containing the information about the socket() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    SocketEvent(gcstring_map& m, gcstring& file, Socket* socket);

    SocketEvent();

    /**
     * Destructor.
     */
    virtual ~SocketEvent();

    /**
     * Calls IModule::imodule_library_socket() and passes this SocketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_socket() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_socket()
     */
    void execute(IModule* m);

    int get_domain();
    int get_type();
    int get_protocol();


};

#endif	/* SOCKETEVENT_H */
