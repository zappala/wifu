/* 
 * File:   Dispatcher.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 2:45 PM
 */

#ifndef _SOCKETDISPATCHER_H
#define	_SOCKETDISPATCHER_H

#include <string>
#include <vector>

#include "LocalSocketFullDuplex.h"

// modules
#include "SocketConnectionManager.h"
#include "SocketReliability.h"
#include "SocketCongestionControl.h"

using namespace std;

class SocketDispatcher : public LocalSocketFullDuplex {
private:
    SocketDispatcher(string & file);
    SocketDispatcher(SocketDispatcher const&);
    SocketDispatcher& operator=(SocketDispatcher const&);

public:
    ~SocketDispatcher();
    static SocketDispatcher & instance();
    void receive(string & message);

    vector<ISocketModule *> modules_;

    


    
};

#endif	/* _DISPATCHER_H */

