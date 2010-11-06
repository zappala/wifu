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

/*
 * @deprecated This class uses Unix sockets; we are now using Events instead.
 */
class SocketUDPInterface : public LocalSocketFullDuplex {
private:

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    static SocketUDPInterface * instance_;

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    SocketUDPInterface(string & file);

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    SocketUDPInterface(SocketUDPInterface const&);

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    SocketUDPInterface & operator=(SocketUDPInterface const&);

public:
    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    ~SocketUDPInterface();

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    static SocketUDPInterface * instance();

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    void receive(string & message);

    /*
     * @deprecated This class uses Unix sockets; we are now using Events instead.
     */
    void destroy(void);
};

#endif	/* _SOCKETUDPINTERFACE_H */

