/* 
 * File:   Event.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:46 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H

#include "Identifiable.h"
#include "IModule.h"

class IModule;

class Event : public Identifiable {
public:
    Event(int socket);
    virtual ~Event();
    virtual void execute(IModule * m) = 0;
    int & get_socket();
private:
    int socket_;
    
};



#endif	/* _EVENT_H */

