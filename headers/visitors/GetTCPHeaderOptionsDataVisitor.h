/* 
 * File:   GetTCPHeaderOptionsDataVisitor.h
 * Author: rbuck
 *
 * Created on April 28, 2011, 1:24 PM
 */

#ifndef GETTCPHEADEROPTIONSDATAVISITOR_H
#define	GETTCPHEADEROPTIONSDATAVISITOR_H

#include "GetTCPHeaderOptionsLengthVisitor.h"


class GetTCPHeaderOptionsDataVisitor : public GetTCPHeaderOptionsLengthVisitor {
public:
    GetTCPHeaderOptionsDataVisitor(unsigned char* options);
    void append_padding();
    void visit(TCPHeaderOption* option);
private:
    unsigned char* options_;
};

#endif	/* GETTCPHEADEROPTIONSDATAVISITOR_H */

