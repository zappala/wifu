/* 
 * File:   BindEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:27 PM
 */

#ifndef BINDEVENT_H
#define	BINDEVENT_H

#include "LibraryEvent.h"

using namespace std;

/**
 * Represents a bind() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class BindEvent : public LibraryEvent {
public:

    /**
     * Constructs a BindEvent.
     *
     * @param message Message received from the front end library containing the information about the bind() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    BindEvent(gcstring& message, gcstring& file, Socket* s);

    /**
     * Destructor.
     */
    virtual ~BindEvent();

    /**
     * Calls IModule::imodule_library_bind() and passes this BindEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_bind() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_bind()
     */
    void execute(IModule* m);

};

#endif	/* BINDEVENT_H */
