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

    /**
     * Constructs a ListenEvent.
     *
     * @param m Map received from the front end library containing the information about the listen() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    ListenEvent(gcstring_map& m, gcstring& file, Socket* s);

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
