/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "AddressEvent.h"
#include "Socket.h"

/**
 * Represents a connect() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class ConnectEvent : public AddressEvent {
public:

    ConnectEvent();

    /**
     * Destructor.
     */
    virtual ~ConnectEvent();

    /**
     * Calls IModule::imodule_library_connect() and passes this CloseEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_connect() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_connect()
     */
    void execute(IModule* m);
};

#endif	/* _CONNECTEVENT_H */
