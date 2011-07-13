/* 
 * File:   CloseEvent.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 5:16 PM
 */

#ifndef CLOSEEVENT_H
#define	CLOSEEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

using namespace std;

/**
 * Represents a close() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class CloseEvent : public LibraryEvent {
public:

    /**
     * Constructs a CloseEvent.
     *
     * @param m Map received from the front end library containing the information about the close() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    CloseEvent(gcstring_map& m, gcstring& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~CloseEvent();

    /**
     * Calls IModule::imodule_library_close() and passes this CloseEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_close() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_close()
     */
    void execute(IModule* m);
};

#endif	/* CLOSEEVENT_H */

