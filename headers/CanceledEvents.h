/* 
 * File:   CanceledEvents.h
 * Author: rbuck
 *
 * Created on October 25, 2010, 10:21 AM
 */

#ifndef _CANCELEDEVENTS_H
#define	_CANCELEDEVENTS_H

#include "EventSet.h"

using namespace std;

class CanceledEvents : public EventSet {
private:
    CanceledEvents();
    CanceledEvents(CanceledEvents const&);
    CanceledEvents& operator=(CanceledEvents const&);

public:
    ~CanceledEvents();
    static CanceledEvents & instance();
    
};

#endif	/* _CANCELEDEVENTS_H */

