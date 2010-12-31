/* 
 * File:   SynSent.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:50 PM
 */

#ifndef SYNSENT_H
#define	SYNSENT_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/Established.h"

using namespace std;

class SynSent : public State {
public:
    SynSent();
    virtual ~SynSent();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void receive(Context* c, string& data);

};

#endif	/* SYNSENT_H */

