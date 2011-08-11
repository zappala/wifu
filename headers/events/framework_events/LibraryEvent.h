/* 
 * File:   LibraryEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:43 PM
 */

#ifndef LIBRARYEVENT_H
#define	LIBRARYEVENT_H

#include <stdlib.h>

#include "defines.h"
#include "BufferEvent.h"
#include "QueryStringParser.h"
#include "Utils.h"
#include "MessageStructDefinitions.h"

using namespace std;

/**
 * Represents an Event which was generated due to an application request such as socket(), bind(), listen(), send(), recv(), etc.
 * Library Events are all given a priority of LOW.
 *
 * @see Event
 * @see FrameworkEvent
 */
class LibraryEvent : public BufferEvent {
public:

    /**
     * Destructor.
     */
    virtual ~LibraryEvent();

    LibraryEvent();
    void save_buffer(unsigned char* buffer, u_int32_t length);
    struct sockaddr_un* get_source() const;
    u_int32_t get_message_type() const;
    u_int32_t get_buffer_length() const;
    int get_fd() const;

private:
    u_int32_t buffer_length_;
    struct FrontEndMessage* message_;
};

#endif	/* LIBRARYEVENT_H */
