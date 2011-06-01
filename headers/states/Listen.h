/* 
 * File:   Listen.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:42 PM
 */

#ifndef LISTEN_H
#define	LISTEN_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "Accept.h"
#include "events/framework_events/AcceptEvent.h"

using namespace std;

class Listen : public State {
public:
    Listen();
    virtual ~Listen();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_accept(Context* c, AcceptEvent* e);

};

#endif	/* LISTEN_H */

