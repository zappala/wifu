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
#include "events/AcceptEvent.h"

using namespace std;

class Listen : public State {
public:
    Listen();
    virtual ~Listen();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void accept(Context* c, AcceptEvent* e);

};

#endif	/* LISTEN_H */

