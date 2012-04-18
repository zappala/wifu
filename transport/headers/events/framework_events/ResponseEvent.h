/* 
 * File:   ResponseEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:36 PM
 */

#ifndef RESPONSEEVENT_H
#define	RESPONSEEVENT_H


#include "BufferEvent.h"
#include "QueryStringParser.h"
#include "Socket.h"
#include "Utils.h"
#include "defines.h"
#include "MessageStructDefinitions.h"

using namespace std;

/**
 * Event that is interpreted by WiFuTransportBackEndTranslator and converted to a message to be passed over a Unix socket to the front end.
 * This Event will cause the BSD socket call to return for a single socket.
 * This is Event is the only Event used to communicate information from the back end to the front end.
 *
 * @see Event
 * @see FrameworkEvent
 * @see WiFuTransportBackEndTranslator
 */
class ResponseEvent : public BufferEvent {
public:

    ResponseEvent();

    /**
     * Destructor.
     */
    virtual ~ResponseEvent();

    void set_message_type(u_int32_t message_type);
    void set_fd(int fd);
    void set_return_value(int return_value);
    void set_errno(int error);
    void set_default_length();
    void set_length(u_int32_t length);
    u_int32_t get_length() const;

    struct sockaddr_un* get_destination();
    void set_destination(struct sockaddr_un* destination);

    struct GenericResponseMessage* get_response();

    /**
     * Calls IModule::imodule_library_response() and passes this ResponseEvent in as the argument.
     * WiFuTransportBackEndTranslator should be the only IModule using this method.
     *
     * @param m The module which to call IModule::imodule_library_response() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_response()
     * @see WiFuTransportBackEndTranslator::imodule_library_response()
     */
    void execute(IModule* m);

private:
    struct GenericResponseMessage* response_;
    struct sockaddr_un destination_;
};

#endif	/* RESPONSEEVENT_H */
