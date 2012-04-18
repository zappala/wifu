/* 
 * File:   SocketCollectionGetByLocalAddressPortVisitor.h
 * Author: rbuck
 *
 * Created on February 2, 2011, 10:12 AM
 */

#ifndef _SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H
#define	_SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H

#include "SocketCollectionVisitor.h"

using namespace std;

class SocketCollectionGetByLocalAddressPortVisitor : public SocketCollectionVisitor {
public:
    SocketCollectionGetByLocalAddressPortVisitor(AddressPort* local);

    virtual ~SocketCollectionGetByLocalAddressPortVisitor();

    void visit(Socket*);

private:
    AddressPort* local_;
};

#endif	/* _SOCKETCOLLECTIONGETBYLOCALADDRESSPORTVISITOR_H */

