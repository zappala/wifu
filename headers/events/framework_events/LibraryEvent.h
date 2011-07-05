/* 
 * File:   LibraryEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:43 PM
 */

#ifndef LIBRARYEVENT_H
#define	LIBRARYEVENT_H

#include <string>
#include <map>
#include <stdlib.h>

#include "defines.h"
#include "FrameworkEvent.h"
#include "QueryStringParser.h"
#include "Utils.h"

using namespace std;

/**
 * Represents an Event which was generated due to an application request such as socket(), bind(), listen(), send(), recv(), etc.
 * Library Events are all given a priority of LOW.
 *
 * @see Event
 * @see FrameworkEvent
 */
class LibraryEvent : public FrameworkEvent {
public:

    /**
     * Constructs a LibraryEvent.
     * Parses message which is of a query string format that was received by WiFuEndBackEndLibrary over a Unix socket from the front end.
     * It parses the message into key value pairs and stores them in a map for easy access.
     * 
     * @param message Message received from the front end library containing the information about the specific BSD socket API call.
     * @param file The file to write any response to the application.  Represents a Unix socket.
     * @param socket The Socket object to which this Event belongs.
     *
     * @see Event
     * @see FrameworkEvent
     * @see QueryStringParser
     * @see WiFuEndBackEndLibrary
     */
    LibraryEvent(string& message, string& file, Socket* socket);

    /**
     * Destructor.
     */
    virtual ~LibraryEvent();

    /**
     * @return The name of the file representing a Unix socket to write any response to the front end to.
     */
    string& get_file();

    /**
     * @return The name of the BSD socket method which this LibraryEvent represents.
     */
    string& get_name();

    /**
     * Returns a map containing the key-value pairs of the information passed from the front end to the back end.
     * The keys for each map pair are the same used by the front end and should be accessed in the same manner in which they were stored.
     * The keys are defined in defines.h.
     *  
     * Overriding Event objects may wish to provide helper methods to retrieve specific values so the user does not need to access the map itself.
     *
     * @return A map containing the key-value pairs of the information passed from the front end to the back end.
     */
    map<string, string>& get_map();

private:

    /**
     * A map containing key-value pairs of the information passed into a BSD socket function call as well as some bookkeeping information.
     */
    map<string, string> m_;

    /**
     * The specific BSD socket function call name (socket, listen, bind, recv, etc.).
     */
    string name_;

    /**
     * File representing a socket to which any ResponseEvent must use to correctly transmit data back to the front end.
     */
    string file_;

};

#endif	/* LIBRARYEVENT_H */
