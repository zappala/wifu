/* 
 * File:   Event.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:46 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H

#include <assert.h>

#include "GarbageCollector.h"
#include "IModule.h"
#include "Socket.h"
#include "exceptions/WiFuException.h"
#include "defines.h"

class IModule;

//TODO: do we get destructors with gc?

/**
 * Represents an Event which IModule objects will receive and react to.
 */
class Event : public gc {
public:
    /**
     * Constructs an Event object.
     *
     * @param socket The socket to which to apply this Event to.
     */
    Event(Socket* s, Priority p = LOW);

    /**
     * Constructs an Event object.
     */
    Event();

    /**
     * Cleans up this Event object.
     */
    virtual ~Event();

    /**
     * Will call a specific method on m depending on the type of this Event.
     *
     * @param m The module which to call a specific method on.
     * @see IModule
     */
    virtual void execute(IModule* m) = 0;

    /**
     * @return A pointer to the socket.
     */
    Socket* get_socket();

    void set_socket(Socket*);
    
    Priority get_priority();

    void set_priority(Priority priority);

private:
    /**
     * Socket which is associated with this Event.
     */
    Socket* socket_;

    /*
     * The priority of this Event
     */
    Priority priority_;
};

#endif	/* _EVENT_H */
