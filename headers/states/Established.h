/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:54 PM
 */

#ifndef ESTABLISHED_H
#define	ESTABLISHED_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "states/Established.h"
#include "events/ResponseEvent.h"
#include "events/ConnectEvent.h"

using namespace std;

class Established : public State {
public:
    Established();
    virtual ~Established();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void receive(Context* c, Socket* s, WiFuPacket* p);

};

#endif	/* ESTABLISHED_H */

