/* 
 * File:   Open.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 1:10 PM
 */

#ifndef OPEN_H
#define	OPEN_H

#include <iostream>

#include "Context.h"


using namespace std;

class Open : public State {
public:
    Open();
    virtual ~Open();
    virtual void enter(Context* c);
    virtual void exit(Context* c);
    virtual void close(Context* c);
    virtual bool is_open();
};

#endif	/* OPEN_H */

