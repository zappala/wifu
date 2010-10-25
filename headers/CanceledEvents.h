/* 
 * File:   CanceledEvents.h
 * Author: rbuck
 *
 * Created on October 25, 2010, 10:21 AM
 */

#ifndef _CANCELEDEVENTS_H
#define	_CANCELEDEVENTS_H

#include <tr1/unordered_set>
#include "Semaphore.h"
#include "Event.h"

using namespace std;
//using namespace tr1;

class CanceledEvents {
private:
    CanceledEvents();
    CanceledEvents(CanceledEvents const&);
    CanceledEvents& operator=(CanceledEvents const&);

    tr1::unordered_set<int> ids_;
    Semaphore sem_;

public:
    ~CanceledEvents();
    static CanceledEvents & instance();
    
    void add(Event * event);
    void remove(Event * event);
    bool is_canceled(Event * event);
};

#endif	/* _CANCELEDEVENTS_H */

