/* 
 * File:   TCPFewIContextContainer.h
 * Author: leer
 *
 * Created on June 28, 2011, 1:20 PM
 */

#ifndef _TCPFEWICONTEXTCONTAINER_H
#define	_TCPFEWICONTEXTCONTAINER_H

#include "contexts/TCPTahoeIContextContainer.h"

class TCPFewIContextContainer : public TCPTahoeIContextContainer {
public:
    TCPFewIContextContainer();
    //TCPFewIContextContainer(const TCPFewIContextContainer& orig);
    virtual ~TCPFewIContextContainer();

    //virtual icontext_socket
};

#endif	/* _TCPFEWICONTEXTCONTAINER_H */

