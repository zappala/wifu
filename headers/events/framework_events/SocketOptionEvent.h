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

using namespace std;

/**
 * Base Event class for getting and setting socket options.
 *
 * @see SetSocketOptionEvent
 * @see GetSocketOptionEvent
 * @see SocketOptions
 */
class SocketOptionEvent : public LibraryEvent {
public:

    /**
     * Constructs a SocketOptionEvent.
     *
     * @param message Message received from the front end library containing the information about the getsockopt() or setsockopt() BSD socket API calls and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    SocketOptionEvent(gcstring& message, gcstring& file, Socket* s);

    /**
     * Destructor
     */
    virtual ~SocketOptionEvent();

    /**
     * @return The option name.
     * @see Man page for getsockopt()
     */
    int get_option_name();

    /**
     * @return The level for this option.
     * @see Man page for getsockopt()
     */
    int get_level();

    /**
     * @return The length of the option passed in.
     */
    socklen_t get_option_length();

    /**
     * @return A pair containing the option level and name.
     * @see SocketOptionEvent::get_level()
     * @see SocketOptionEvent::get_option_name()
     */
    pair<int, int> get_level_name_pair();
};

#endif	/* SOCKETOPTIONEVENT_H */

