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

    BindEvent();

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

    struct sockaddr_in* get_addr();
    socklen_t get_addr_len();

};

#endif	/* BINDEVENT_H */
