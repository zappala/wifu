/* 
 * File:   ReceiveEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _RECEIVEEVENT_H
#define	_RECEIVEEVENT_H

#include "DataEvent.h"

using namespace std;

/**
 * Represents a recv() or recvfrom() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class ReceiveEvent : public DataEvent {
public:

    ReceiveEvent();

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
};

#endif	/* _RECEIVEEVENT_H */

