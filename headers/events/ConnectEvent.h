/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "LibraryEvent.h"

/**
 * Event which represents a connect event.
 * @see Event
 */
class ConnectEvent : public LibraryEvent {

public:

    /**
     * Constructs a ConnectEvent.
     *
     */
    ConnectEvent(string& message, string& file) : LibraryEvent(message, file) {

    }

    virtual ~ConnectEvent() {

    }

    /**
     * Will call connect() on m.
     *
     * @param m The IModule which to call connect() on.
     * @see IModule::connect()
     */
    void execute(IModule* m) {
        m->library_connect(this);
    }
};

#endif	/* _CONNECTEVENT_H */

