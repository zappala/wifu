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

using namespace std;

class IModule;

//TODO: do we get destructors with gc?

/**
 * Represents an Event which IModule objects will receive and react to.
 * @see IModule
 */
class Event : public gc {
public:
    /**
     * Constructs an Event object.
     *
     * @param s The socket to which to apply this Event to.
     * @param p The Priority of this Event
     */
    Event(Socket* s, Priority p);

    /**
     * Constructs an Event object used only for testing.
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
     * @return A pointer to the socket to which this Event belongs.
     * @throw WiFuException if NULL would be returned.
     */
    Socket* get_socket();

    /**
     * Saves the socket to which this Event belongs.
     * @param s The Socket to which this Event belongs.
     */
    void set_socket(Socket* s);

    /**
     * @return The Priority of this Event for use in any priority queues.
     */
    Priority get_priority();

    /**
     * Sets the Priority of this Event for use in any priority queues.
     * @param priority The Priority of this Event for use in any priority queues.
     */
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
