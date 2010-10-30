/* 
 * File:   UDPInterface.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:46 AM
 */

#ifndef _SOCKETUDPINTERFACE_H
#define	_SOCKETUDPINTERFACE_H

#include "LocalSocketFullDuplex.h"


using namespace std;

class SocketUDPInterface : public LocalSocketFullDuplex {
private:
    static SocketUDPInterface * instance_;
    SocketUDPInterface(string & file);
    SocketUDPInterface(SocketUDPInterface const&);
    SocketUDPInterface & operator=(SocketUDPInterface const&);

public:
    ~SocketUDPInterface();
    static SocketUDPInterface * instance();
    void receive(string & message);
    void destroy(void);
};

#endif	/* _SOCKETUDPINTERFACE_H */

