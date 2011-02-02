/* 
 * File:   Observable.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 2:27 PM
 */

#ifndef _OBSERVABLE_H
#define	_OBSERVABLE_H

#include "Observer.h"
#include <set>

using namespace std;

class Observable {
private:
    set<class Observer*> observers_;
    set<class Observer*>::iterator itr_;

public:
    Observable();
    virtual ~Observable();
    void add_observer(class Observer* o);
    void notify();

    
};

#endif	/* _OBSERVABLE_H */

