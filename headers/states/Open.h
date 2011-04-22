/* 
 * File:   Open.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 1:10 PM
 */

#ifndef OPEN_H
#define	OPEN_H

#include <iostream>

#include "contexts/Context.h"
#include "states/Closed.h"

using namespace std;

class Open : public State {
public:
    Open();
    virtual ~Open();
    virtual void close(Context* c);
};

#endif	/* OPEN_H */

