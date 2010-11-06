/* 
 * File:   Reliability.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 2:26 PM
 */

#ifndef _SOCKETRELIABILITY_H
#define	_SOCKETRELIABILITY_H

#include <iostream>

#include "LocalSocketFullDuplex.h"
#include "ISocketModule.h"

/*
 * @depricated This class uses Unix sockets; we are now using Events instead.
 */
class SocketReliability : public LocalSocketFullDuplex, public ISocketModule {
public:
    SocketReliability(string & file);
    virtual ~SocketReliability();

    void receive(string &  message);

    void connect(string & message);
    void data(string & message);
};


#endif	/* _SOCKETRELIABILITY_H */

