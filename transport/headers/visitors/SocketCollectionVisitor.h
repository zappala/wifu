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
    SocketCollectionVisitor();
    
    virtual ~SocketCollectionVisitor();
    
    void set_socket(Socket* s);
    
    Socket* get_socket();
    
    bool stop();

private:
    Socket* socket_;
};

#endif	/* _SOCKETCOLLECTIONVISITOR_H */

