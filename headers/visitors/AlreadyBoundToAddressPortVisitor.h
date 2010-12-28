/* 
 * File:   AlreadyBoundToAddressPortVisitor.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:59 AM
 */

#ifndef ALREADYBOUNDTOADDRESSPORTVISITOR_H
#define	ALREADYBOUNDTOADDRESSPORTVISITOR_H

#include "visitors/Visitor.h"

/**
 * Visitor used to check if any Sockets are already bound to an AddressPort or not
 * @see Visitor
 */
class AlreadyBoundToAddressPortVisitor : public Visitor {
public:

    /**
     * Constructs a new object.
     * @param ap Pointer to an AddressPort which will be compared with all Sockets to see if it is in use or not.
     */
    AlreadyBoundToAddressPortVisitor(AddressPort* ap) : is_bound_(false), ap_(ap) {
        
    }

    /**
     * Does nothing special
     */
    virtual ~AlreadyBoundToAddressPortVisitor() {

    }

    /**
     * If socket's local port is equal to that which was passed in to the constructor is_bound() will be true, false otherwise.
     * @param Pointer to current Socket to check
     * @see Socket
     * @see AddressPort
     */
    void visit(Socket* socket) {
        if(socket->get_local_address_port()->operator ==(*ap_)) {
            is_bound_ = true;
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
        return is_bound();
    }

    /**
     * Returns whether there are any Socket objects in use which are bound to the AddressPort passed into the constructor.
     * @return Whether there are any Socket objects in use which are bound to the AddressPort passed into the constructor.
     */
    bool is_bound() {
        return is_bound_;
    }



private:
    AddressPort* ap_;
    bool is_bound_;
};

#endif	/* ALREADYBOUNDTOADDRESSPORTVISITOR_H */

