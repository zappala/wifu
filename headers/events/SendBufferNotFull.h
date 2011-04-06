/* 
 * File:   SendBufferNotFull.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTFULL_H
#define	SENDBUFFERNOTFULL_H

#include "Event.h"

class SendBufferNotFull : public Event {
public:
    SendBufferNotFull(Socket* s);

    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTFULL_H */

