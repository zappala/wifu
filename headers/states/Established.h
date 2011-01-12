/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:54 PM
 */

#ifndef ESTABLISHED_H
#define	ESTABLISHED_H

#include "../contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "../states/Established.h"

using namespace std;

class Established : public State {
public:
    Established();
    virtual ~Established();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void receive(Context* c, Socket* s, Packet* p);

};

#endif	/* ESTABLISHED_H */

