/* 
 * File:   SocketCollectionGetByLocalAddressPortVisitor.h
 * Author: rbuck
 *
 * Created on February 2, 2011, 10:12 AM
 */

#ifndef _SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H
#define	_SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H

#include "Visitor.h"

using namespace std;

class SocketCollectionGetByLocalAddressPortVisitor : public SocketCollectionVisitor {
public:

    SocketCollectionGetByLocalAddressPortVisitor(AddressPort* local) : local_(local) {

    }

    virtual ~SocketCollectionGetByLocalAddressPortVisitor() {

    }

    void visit(Socket* s) {
        if ((*s->get_local_address_port()) == (*local_)) {
            set_socket(s);
        }
    }

private:
    AddressPort* local_;
};

#endif	/* _SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H */

