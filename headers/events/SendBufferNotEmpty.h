/* 
 * File:   SendBufferNotEmpty.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTEMPTY_H
#define	SENDBUFFERNOTEMPTY_H

#include "Event.h"

class SendBufferNotEmpty : public Event {
public:
    SendBufferNotEmpty(Socket* s);

    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTEMPTY_H */

