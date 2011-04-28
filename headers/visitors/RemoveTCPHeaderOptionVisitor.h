/* 
 * File:   FindTCPHeaderOptionVisitor.h
 * Author: rbuck
 *
 * Created on April 28, 2011, 11:32 AM
 */

#ifndef FINDTCPHEADEROPTIONVISITOR_H
#define	FINDTCPHEADEROPTIONVISITOR_H

#include "Visitor.h"
#include "packet/TCPHeaderOption.h"

class FindTCPHeaderOptionVisitor : public Visitor {
public:
    FindTCPHeaderOptionVisitor(u_int8_t kind);
    void visit(TCPHeaderOption* option);
    bool stop();
    TCPHeaderOption* get_option();

private:
    u_int8_t kind_;
    TCPHeaderOption* option_;
};

#endif	/* FINDTCPHEADEROPTIONVISITOR_H */

