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

    CloseEvent();

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

