/* 
 * File:   Dispatcher.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 2:45 PM
 */

#ifndef _DISPATCHER_H
#define	_DISPATCHER_H

#include <string>
#include <vector>

#include "LocalSocketFullDuplex.h"

// modules
#include "ConnectionManager.h"
#include "Reliability.h"
#include "CongestionControl.h"

using namespace std;

class Dispatcher : public LocalSocketFullDuplex {
private:
    static Dispatcher * instance_;
    Dispatcher(string & file);
    Dispatcher(Dispatcher const&);
    Dispatcher& operator=(Dispatcher const&);

public:
    ~Dispatcher();
    static Dispatcher * instance();
    void receive(string & message);
    void destroy(void);

    vector<IModule *> modules_;

    


    
};

#endif	/* _DISPATCHER_H */

