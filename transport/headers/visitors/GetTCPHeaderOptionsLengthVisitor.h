/* 
 * File:   GetTCPHeaderOptionsLengthVisitor.h
 * Author: rbuck
 *
 * Created on April 28, 2011, 11:53 AM
 */

#ifndef GETTCPHEADEROPTIONSLENGTHVISITOR_H
#define	GETTCPHEADEROPTIONSLENGTHVISITOR_H

#include <cmath>
#include "Visitor.h"

class GetTCPHeaderOptionsLengthVisitor : public Visitor {
public:
    GetTCPHeaderOptionsLengthVisitor();
    u_int8_t get_padded_length();
    u_int8_t get_num_padded_bytes();
    void visit(TCPHeaderOption* option);
private:
    int length_bytes_;
};

#endif	/* GETTCPHEADEROPTIONSLENGTHVISITOR_H */

