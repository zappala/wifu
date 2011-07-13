/* 
 * File:   ReceiveEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _RECEIVEEVENT_H
#define	_RECEIVEEVENT_H

#include "LibraryEvent.h"

using namespace std;

/**
 * Represents a recv() or recvfrom() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class ReceiveEvent : public LibraryEvent {
public:

    /**
     * Constructs a ReceiveEvent.
     *
     * @param m Map received from the front end library containing the information about the recv() or recvfrom() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    ReceiveEvent(gcstring_map& m, gcstring& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~ReceiveEvent();

    /**
     * Calls IModule::imodule_library_receive() and passes this ReceiveEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_receive() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_receive()
     */
    void execute(IModule* m);

    /**
     * @return The maximum number of bytes that the application is willing to receive.
     */
    int get_receive_buffer_size();
};

#endif	/* _RECEIVEEVENT_H */

