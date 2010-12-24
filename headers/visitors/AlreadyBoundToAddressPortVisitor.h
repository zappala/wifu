/* 
 * File:   AlreadyBoundToAddressPortVisitor.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:59 AM
 */

#ifndef ALREADYBOUNDTOADDRESSPORTVISITOR_H
#define	ALREADYBOUNDTOADDRESSPORTVISITOR_H

#include "visitors/Visitor.h"

class AlreadyBoundToAddressPortVisitor : public Visitor {
public:
    AlreadyBoundToAddressPortVisitor(AddressPort& ap) : is_bound_(false) {
        ap_ = new AddressPort(ap);
    }

    virtual ~AlreadyBoundToAddressPortVisitor() {
        delete ap_;
    }

    void visit(Socket* s) {
        if(s->get_local_address_port()->operator ==(*ap_)) {
            is_bound_ = true;
        }
    }

    bool is_bound() {
        return is_bound_;
    }

private:
    AddressPort* ap_;
    bool is_bound_;
};

#endif	/* ALREADYBOUNDTOADDRESSPORTVISITOR_H */

