/* 
 * File:   ReceiveBufferNotEmpty.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef RECEIVEBUFFERNOTEMPTY_H
#define	RECEIVEBUFFERNOTEMPTY_H

#include "Event.h"

class ReceiveBufferNotEmpty : public Event {
public:
    ReceiveBufferNotEmpty(Socket* s);

    void execute(IModule* m);
};

#endif	/* RECEIVEBUFFERNOTEMPTY_H */

