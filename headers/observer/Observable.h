/* 
 * File:   Observable.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 2:27 PM
 */

#ifndef _OBSERVABLE_H
#define	_OBSERVABLE_H

#include "Observer.h"
#include <vector>

using namespace std;

class Observable {
private:
    vector<class Observer*> observers_;

public:
    Observable();
    virtual ~Observable();
    void add_observer(class Observer* o);
    void notify();

    
};

#endif	/* _OBSERVABLE_H */

