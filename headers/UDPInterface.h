/* 
 * File:   UDPInterface.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:46 AM
 */

#ifndef _UDPINTERFACE_H
#define	_UDPINTERFACE_H

#include "LocalSocketFullDuplex.h"


using namespace std;

class UDPInterface : public LocalSocketFullDuplex {
private:
    static UDPInterface * instance_;
    UDPInterface(string & file);
    UDPInterface(UDPInterface const&);
    UDPInterface & operator=(UDPInterface const&);

public:
    ~UDPInterface();
    static UDPInterface * instance();
    void receive(string & message);
    void destroy(void);
};

#endif	/* _UDPINTERFACE_H */

