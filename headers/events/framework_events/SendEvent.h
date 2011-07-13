/* 
 * File:   SendEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _SENDEVENT_H
#define	_SENDEVENT_H

#include "LibraryEvent.h"

using namespace std;

/**
 * Represents a send() or sendto() call by the application.
 *
 * @see Event
 * @see FrameworkEvent
 * @see LibraryEvent
 *
 */
class SendEvent : public LibraryEvent {
public:

    /**
     * Constructs a SendEvent.
     *
     * @param m Map received from the front end library containing the information about the send() or sendto() BSD socket API call and some bookkeeping information.
     * @param file The file associated with a Unix socket to write any response to the application.
     * @param s The Socket object to which this Event belongs.
     */
    SendEvent(gcstring_map& m, gcstring& file, Socket* s);

    /**
     * Calls IModule::imodule_library_send() and passes this SendEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_library_send() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_send()
     */
    void execute(IModule* m);

    /**
     * @return A pointer to the data buffer to send.
     */
    unsigned char* get_data();

    /**
     * @return The length (in bytes) of the data to send.
     */
    ssize_t data_length();

private:

    /**
     * Pointer to the buffer of data to send.
     */
    unsigned char * data_;

    /**
     * The length (in bytes) of the data to send.
     */
    ssize_t data_length_;
};

#endif	/* _SENDEVENT_H */

