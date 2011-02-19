/* 
 * File:   AcceptEvent.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 10:49 AM
 */

#ifndef _ACCEPTEVENT_H
#define	_ACCEPTEVENT_H

#include "LibraryEvent.h"

class AcceptEvent : public LibraryEvent {
public:
    AcceptEvent(string&, string&, Socket*);

    virtual ~AcceptEvent();

    void execute(IModule*);

};

#endif	/* _ACCEPTEVENT_H */
