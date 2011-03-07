/* 
 * File:   SendEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _SENDEVENT_H
#define	_SENDEVENT_H

#include "LibraryEvent.h"


class SendEvent : public LibraryEvent {
public:
    SendEvent(string& message, string& file, Socket* s);
    void execute(IModule* m);

    unsigned char * get_data();
    ssize_t data_length();

private:
    unsigned char * data_;
    ssize_t data_length_;
};

#endif	/* _SENDEVENT_H */

