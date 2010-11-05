/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IMODULE_H
#define	_IMODULE_H

#include "Event.h"
#include <iostream>

class Event;

using namespace std;

/**
 * Contains all possible event actions which could occur.
 * This is technically NOT an interface as a default (empty)
 * method body is provided.  This is so a class which inherits
 * from this class does not have to implement all methods.
 */
class IModule {
public:

    /**
     * Constructs an IModule.
     */
    IModule() {

    }

    /**
     * Cleans up an IModule.
     */
    virtual ~IModule(){

    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a timeout action.
     * This method should call my_timer_fired() upon determining that e is for this IModule.
     *
     * @param e The event which triggered a timeout action.
     */
    virtual void timeout(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a timer fired action.
     *
     * @param e The event which triggered a timer fired action.
     */
    virtual void timer_fired(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a cancel timer action.
     *
     * @param e The event which triggered a cancel timer action.
     */
    virtual void cancel_timer(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a connect action.
     *
     * @param e The event which triggered a connect action.
     */
    virtual void connect(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a udp send action.
     *
     * @param e The event which triggered a udp send action.
     */
    virtual void udp_send(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a data action.
     *
     * @param e The event which triggered a data action.
     */
    virtual void data(Event* e) {}

    /**
     * Called when e's execute() method determines that this IModule should respond to a receive action.
     *
     * @param e The event which triggered a receive action.
     */
    virtual void receive(Event* e) {}

protected:

    /**
     * Should be called by timeout() upon determining that e is for this IModule.
     *
     * @param e The event which triggered a timeout action and has been determined to be for this IModule.
     */
    virtual void my_timer_fired(Event* e) {}

};

#endif	/* _IMODULE_H */

