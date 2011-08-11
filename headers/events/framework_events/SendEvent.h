/* 
 * File:   SendEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _SENDEVENT_H
#define	_SENDEVENT_H

#include "DataEvent.h"

using namespace std;

/**
 * Represents a send() or sendto() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class SendEvent : public DataEvent {
public:

    SendEvent();

    /**
     * Calls IModule::imodule_library_send() and passes this SendEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_send() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_send()
     */
    void execute(IModule* m);

    /**
     * @return A pointer to the data buffer to send.
     */
    unsigned char* get_data();
};

#endif	/* _SENDEVENT_H */

