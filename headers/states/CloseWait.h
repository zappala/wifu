/* 
 * File:   CloseWait.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:41 PM
 */

#ifndef CLOSEWAIT_H
#define	CLOSEWAIT_H

#include "State.h"
#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

class CloseWait : public State {
public:
    CloseWait();
    virtual ~CloseWait();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    virtual void close(Context* c);
};

#endif	/* CLOSEWAIT_H */

