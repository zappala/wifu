/* 
 * File:   AlreadyListeningOnSamePortVisitor.h
 * Author: rbuck
 *
 * Created on December 28, 2010, 1:47 PM
 */

#ifndef _ALREADYLISTENINGONSAMEPORTVISITOR_H
#define	_ALREADYLISTENINGONSAMEPORTVISITOR_H

#include "Visitor.h"

/**
 * Visitor used to check if any Sockets are already listening on a port
 * @see Visitor
 */
class AlreadyListeningOnSamePortVisitor : public Visitor {
public:

    /**
     * Constructor
     *
     * @param port Port that this visitor will compare against
     */
    AlreadyListeningOnSamePortVisitor(u_int16_t port) : is_listening_(false), port_(port) {
        
    }

    /**
     * Does nothing special
     */
    virtual ~AlreadyListeningOnSamePortVisitor() {
        
    }

    /**
     * Method called for each Socket in the SocketCollection
     * Examines the Socket to see if the port (passed into the constructor) is valid or not
     * @param socket Pointer to a Socket to check
     * @see Socket
     * @see AddressPort
     */
    void visit(Socket* socket) {
        if( socket->is_passive() &&
            socket->get_local_address_port()->get_port() == port_) {

            is_listening_ = true;
        }
    }

    /**
     * Returns whether the Visitable object should stop iterating over the objects.
     * @return Whether the Visitable object should stop iterating over the objects.
     * @see Visitor::stop()
     * @see Visitable
     *
     */
    bool stop() {
        return is_listening();
    }

    /**
     * Returns whether there are any Socket objects in use which are listening to the port passed into the constructor.
     * @return whether there are any Socket objects in use which are listening to the port passed into the constructor.
     */
    bool is_listening() {
        return is_listening_;
    }


private:
    u_int16_t port_;
    bool is_listening_;
};

#endif	/* _ALREADYLISTENINGONSAMEPORTVISITOR_H */

