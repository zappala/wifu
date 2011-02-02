/* 
 * File:   SocketCollectionGetByIdVisitor.h
 * Author: rbuck
 *
 * Created on February 2, 2011, 9:50 AM
 */

#ifndef _SOCKETCOLLECTIONGETBYIDVISITOR_H
#define	_SOCKETCOLLECTIONGETBYIDVISITOR_H

#include "Visitor.h"
#include "SocketCollectionVisitor.h"

using namespace std;

class SocketCollectionGetByIdVisitor : public SocketCollectionVisitor {
public:

    SocketCollectionGetByIdVisitor(int id) : id_(id) {

    }

    virtual ~SocketCollectionGetByIdVisitor() {

    }

    void visit(Socket* s) {
        if(s->get_socket_id() == id_) {
            set_socket(s);
        }
    }

private:
    int id_;
};

#endif	/* _SOCKETCOLLECTIONGETBYIDVISITOR_H */

