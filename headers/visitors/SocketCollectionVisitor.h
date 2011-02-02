/* 
 * File:   SocketCollectionVisitor.h
 * Author: rbuck
 *
 * Created on February 2, 2011, 10:15 AM
 */

#ifndef _SOCKETCOLLECTIONVISITOR_H
#define	_SOCKETCOLLECTIONVISITOR_H

#include "Visitor.h"

using namespace std;

class SocketCollectionVisitor : public Visitor {
public:
    SocketCollectionVisitor() : Visitor(), socket_(0) {
        
    }
    
    virtual ~SocketCollectionVisitor() {
        
    }
    
    void set_socket(Socket* s) {
        socket_ = s;
    }
    
    Socket* get_socket() {
        return socket_;
    }
    
    bool stop() {
        return socket_ != NULL;
    }    
    
private:
    Socket* socket_;

};

#endif	/* _SOCKETCOLLECTIONVISITOR_H */

