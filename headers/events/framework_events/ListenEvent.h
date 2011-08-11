/* 
 * File:   ListenEvent.h
 * Author: rbuck
 *
 * Created on December 28, 2010, 10:35 AM
 */

#ifndef _LISTENEVENT_H
#define	_LISTENEVENT_H

#include "LibraryEvent.h"

using namespace std;

/**
 * Represents a listen() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class ListenEvent : public LibraryEvent {
public:

    ListenEvent();

    /**
     * Destructor.
     */
    virtual ~ListenEvent();

    /**
     * Calls IModule::imodule_library_listen() and passes this ListenEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_listen() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_listen()
     */
    void execute(IModule* m);

    /**
     * @return The number of pending connections to queue.
     * @see Man page for listen.
     */
    int get_back_log();

};

#endif	/* _LISTENEVENT_H */
