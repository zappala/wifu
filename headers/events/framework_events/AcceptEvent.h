/* 
 * File:   AcceptEvent.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 10:49 AM
 */

#ifndef _ACCEPTEVENT_H
#define	_ACCEPTEVENT_H

#include "AddressEvent.h"

using namespace std;

/**
 * Represents an accept() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 * 
 */
class AcceptEvent : public AddressEvent {
public:

    AcceptEvent();

    /**
     * Destructor.
     */
    virtual ~AcceptEvent();

    /**
     * Calls IModule::imodule_library_accept() and passes this AcceptEvent in as the argument.
     * 
     * @param m The module which to call IModule::imodule_library_accept() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_accept()
     */
    void execute(IModule* m);

};

#endif	/* _ACCEPTEVENT_H */
