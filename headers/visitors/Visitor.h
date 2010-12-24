/* 
 * File:   Visitor.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 9:33 AM
 */

#ifndef VISITOR_H
#define	VISITOR_H

#include "Socket.h"

// need to forward declare all visitable objects
class Socket;

class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(Socket* s) {}
};

#endif	/* VISITOR_H */

