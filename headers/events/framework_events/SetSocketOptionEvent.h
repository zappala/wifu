/* 
 * File:   SetSocketOptionEvent.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 1:40 PM
 */

#ifndef SETSOCKETOPTIONEVENT_H
#define	SETSOCKETOPTIONEVENT_H

#include "SocketOptionEvent.h"
#include "Socket.h"

class SetSocketOptionEvent : public SocketOptionEvent {
public:
    SetSocketOptionEvent(string& message, string& file, Socket* s);
    virtual ~SetSocketOptionEvent();

    void execute(IModule* m);
    string& get_option_value();

    /**
     * Helper function to create the value pair used in the SocketOptions map.
     * @return A pair of value and value-length for this socket option event.
     * @see SocketOptions
     */
    pair<string, socklen_t> get_value_length_pair();
};

#endif	/* SETSOCKETOPTIONEVENT_H */