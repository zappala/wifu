/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IMODULE_H
#define	_IMODULE_H

#include "events/Event.h"
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
    virtual ~IModule() {

    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a timeout action.
     * This method should call my_timer_fired() upon determining that e is for this IModule.
     * This only occurs in the TimeoutEventManager
     *
     * @param e The event which triggered a timeout action.
     */
    virtual void timeout(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a timer fired action.
     *
     * @param e The event which triggered a timer fired action.
     */
    virtual void timer_fired(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a cancel timer action.
     *
     * @param e The event which triggered a cancel timer action.
     */
    virtual void cancel_timer(Event* e) {
    }


    /**
     * Called when e's execute() method determines that this IModule should respond to a network send action.
     *
     * @param e The event which triggered a network send action.
     */
    virtual void network_send(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a network receive action.
     *
     * @param e The event which triggered a network send action.
     */
    virtual void network_receive(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a udp send action.
     *
     * @param e The event which triggered a udp send action.
     */
    virtual void udp_send(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a udp receive action.
     *
     * @param e The event which triggered a udp send action.
     */
    virtual void udp_receive(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a data action.
     *
     * @param e The event which triggered a data action.
     */
    virtual void data(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a send action.
     *
     * @param e The event which triggered a send action.
     */
    virtual void send(Event* e) {
        
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a resend action.
     *
     * @param e The event which triggered a resend action.
     */
    virtual void resend(Event* e) {
        
    }

    // <editor-fold defaultstate="collapsed" desc="Library Methods">

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_socket action.
     *
     * @param e The event which triggered a library_socket action.
     */
    virtual void library_socket(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_bind action.
     *
     * @param e The event which triggered a library_bind action.
     */
    virtual void library_bind(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_listen action.
     *
     * @param e The event which triggered a library_listen action.
     */
    virtual void library_listen(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_accept action.
     *
     * @param e The event which triggered a library_accept action.
     */
    virtual void library_accept(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_send action.
     *
     * @param e The event which triggered a library_send action.
     */
    virtual void library_send(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_receive action.
     *
     * @param e The event which triggered a library_receive action.
     */
    virtual void library_receive(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_close action.
     *
     * @param e The event which triggered a library_close action.
     */
    virtual void library_close(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_connect action.
     *
     * @param e The event which triggered a library_connect action.
     */
    virtual void library_connect(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a library_response action.
     *
     * @param e The event which triggered a library_response action.
     */
    virtual void library_response(Event* e) {
    }
    // </editor-fold>

    /**
     * Called when e's execute() method determines that this IModule should respond to a connection established action.
     *
     * @param e The event which triggered a connection established action.
     */
    virtual void connection_established(Event* e) {
    }

    /**
     * Called when e's execute() method determines that this IModule should respond to a connection initiated action.
     *
     * @param e The event which triggered a connection established action.
     */
    virtual void connection_initiated(Event* e) {
    }

    virtual void send_buffer_not_full(Event* e) {

    }

    virtual void send_buffer_not_empty(Event* e) {

    }

    virtual void receive_buffer_not_empty(Event* e) {

    }


};

#endif	/* _IMODULE_H */

