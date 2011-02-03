/* 
 * File:   CanceledEvents.h
 * Author: rbuck
 *
 * Created on October 25, 2010, 10:21 AM
 */

#ifndef _CANCELEDEVENTS_H
#define	_CANCELEDEVENTS_H

#include "HashSet.h"
#include "events/Event.h"

using namespace std;

/**
 * Singleton class used by TimeoutEventManager to keep track of canceled TimeoutEvent objects.
 */
class CanceledEvents : public HashSet<Event*> {
private:
    /**
     * Constructs a CanceledEvents object.
     */
    CanceledEvents();

    /**
     * Currently not implemented.
     */
    CanceledEvents(CanceledEvents const&);

    /**
     * Currently not implemented.
     */
    CanceledEvents& operator=(CanceledEvents const&);

public:
    /**
     * Cleans up a CanceledEvents object.
     */
    ~CanceledEvents();

    /**
     * @return A reference to the static instance of this CanceledEvents object.
     */
    static CanceledEvents& instance();
    
};

#endif	/* _CANCELEDEVENTS_H */

