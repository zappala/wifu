/* 
 * File:   ResponseEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:36 PM
 */

#ifndef RESPONSEEVENT_H
#define	RESPONSEEVENT_H


#include "FrameworkEvent.h"
#include "QueryStringParser.h"
#include "Socket.h"
#include "Utils.h"
#include "defines.h"

using namespace std;

/**
 * Event that is interpreted by WiFuEndBackEndLibrary and converted to a message to be passed over a Unix socket to the front end.
 * This Event will cause the BSD socket call to return for a single socket.
 * This is Event is the only Event used to communicate information from the back end to the front end.
 *
 * @see Event
 * @see FrameworkEvent
 * @see WiFuEntBackEndLibrary
 */
class ResponseEvent : public FrameworkEvent {
public:

    /**
     * Constructs a ResponseEvent
     * @param socket The Socket object to which this Event belongs.
     * @param name Name of the BSD socket function which we are responding to (socket, bind, listend, recv, etc.).
     * @param file The location of the file the Unix socket is going to receive this response on.
     */
    ResponseEvent(Socket* socket, gcstring& name, gcstring& file);

    /**
     * Destructor.
     */
    virtual ~ResponseEvent();

    /**
     * @return A query string-like message to be passed over a Unix socket to the front end.
     */
    gcstring get_response();

    /**
     * Inserts a key-value pair into the response.
     * @param key The key of the pair.
     * @param value The value of the pair.
     */
    void put(gcstring key, gcstring value);

    /**
     * Inserts a key-value pair into the response.
     * @param key The key of the pair.
     * @param value The value of the pair.
     */
    void put(const char* key, gcstring value);

    /**
     * Calls IModule::imodule_library_response() and passes this ResponseEvent in as the argument.
     * WifuEndBackEndLibrary should be the only IModule using this method.
     *
     * @param m The module which to call IModule::imodule_library_response() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_library_response()
     * @see WifuEndBackEndLibrary::imodule_library_response()
     */
    void execute(IModule* m);

    /**
     * @return A reference to the file used by a Unix socket that this ResponseEvent will send to.
     */
    gcstring get_write_file();

private:

    /**
     * Name of the BSD socket function we are responding to.
     */
    gcstring name_;

    /**
     * The file used by a Unix socket that this ResponseEvent will send to.
     */
    gcstring file_;

    /**
     * A map of the key-value pairs to send to the front end.
     */
    gcstring_map m_;
            
};

#endif	/* RESPONSEEVENT_H */
