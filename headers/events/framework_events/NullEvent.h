/* 
 * File:   NullEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:49 AM
 */

#ifndef _NULLEVENT_H
#define	_NULLEVENT_H

#include "FrameworkEvent.h"

/**
 * Event used in testing only.
 * Simply provides an empty implementation of Event::execute()
 */
class NullEvent : public FrameworkEvent {
public:

    /**
     * Constructs a NullEvent.
     * @param s The Socket object to which this Event belongs.
     */
    NullEvent(Socket* s);

    /**
     * Emtpy implementation.
     * @param m An IModule.
     */
    void execute(IModule* m);

};

#endif	/* _NULLEVENT_H */
