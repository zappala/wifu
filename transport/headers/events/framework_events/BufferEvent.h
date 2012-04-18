/* 
 * File:   BufferEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 10:36 AM
 */

#ifndef BUFFEREVENT_H
#define	BUFFEREVENT_H

#include "FrameworkEvent.h"

class BufferEvent : public FrameworkEvent {
public:
    BufferEvent();
    virtual ~BufferEvent();

    unsigned char* get_buffer();
    int buffer_capacity();

private:
    unsigned char buffer_[UNIX_SOCKET_MAX_BUFFER_SIZE];
};

#endif	/* BUFFEREVENT_H */

