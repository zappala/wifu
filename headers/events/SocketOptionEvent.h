/* 
 * File:   SocketOptionEvent.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 2:07 PM
 */

#ifndef SOCKETOPTIONEVENT_H
#define	SOCKETOPTIONEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

class SocketOptionEvent : public LibraryEvent {
public:
    SocketOptionEvent(string& message, string& file, Socket* s);
    virtual ~SocketOptionEvent();

    int get_option_name();
    int get_level();
    socklen_t get_option_length();
};

#endif	/* SOCKETOPTIONEVENT_H */

