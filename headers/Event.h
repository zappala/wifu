/* 
 * File:   Event.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:46 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H

#include "Identifiable.h"
#include "IQModule.h"

class IQModule;

class Event : public Identifiable {
public:
    Event();
    virtual ~Event();
    virtual void execute(IQModule * m) = 0;

private:
    
};



#endif	/* _EVENT_H */

