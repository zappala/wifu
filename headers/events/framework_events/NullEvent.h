/* 
 * File:   NullEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:49 AM
 */

#ifndef _NULLEVENT_H
#define	_NULLEVENT_H

#include "FrameworkEvent.h"

class NullEvent : public FrameworkEvent {
public:
    NullEvent(Socket* s);

    void execute(IModule* m);

};

#endif	/* _NULLEVENT_H */
