/* 
 * File:   Visitor.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:33 AM
 */

#ifndef VISITOR_H
#define	VISITOR_H

#include "Socket.h"
#include "packet/TCPHeaderOption.h"
#include "exceptions/IllegalStateException.h"

// need to forward declare all visitable objects
class Socket;

/**
 * Interface which supports visiting various objects
 */
class Visitor {
public:
    /**
     * Constructor
     */
    Visitor();
    
    /**
     * Does nothing special
     */
    virtual ~Visitor();

    /**
     * Enbables Sockets to be visited
     * @param s Socket to visit
     */
    virtual void visit(Socket* s);

    /**
     * Enables TCPHeaderOptions to be visited
     * @param option TCPHeaderOption to visit
     */
    virtual void visit(TCPHeaderOption* option);

    /**
     * Provides a way by which a Visitable class may stop early if it is no longer necessary to continue.
     * This is useful if we are searching for a simple predicate to be true/false; we don't need to
     * continue if that predicate is met.
     * @return Whether the Visitable object should stop visiting objects.  Default implementation returns false.
     */
    virtual bool stop();

};

#endif	/* VISITOR_H */

