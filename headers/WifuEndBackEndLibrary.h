/* 
 * File:   WifuEndBackEndLibrary.h
 * Author: rbuck
 *
 * Created on November 26, 2010, 2:19 PM
 */

#ifndef WIFUENDBACKENDLIBRARY_H
#define	WIFUENDBACKENDLIBRARY_H

#include "LocalSocketFullDuplex.h"
#include "Module.h"


class WifuEndBackEndLibrary : public LocalSocketFullDuplex, Module {
public:

    WifuEndBackEndLibrary() {

    }

    virtual ~WifuEndBackEndLibrary() {
        
    }

    /**
     * Callback function when the Unix socket receives a message
     * Will convert the message to an Event and queue it onto the Dispatcher
     * The guts of this method are copied from test/WifuEndAPItest.h
     *
     * @param message Message received from the front-end API
     */
    void receive(string& message) {
        map<string, string> m;
        QueryStringParser::parse(message, m);

        map<string, string> response;
        response[FILE_STRING] = getFile();
        string name = m[NAME_STRING];
        response[SOCKET_STRING] = m[SOCKET_STRING];

        if (!name.compare(WIFU_SOCKET_NAME)) {
            int socket = 1;
            response[SOCKET_STRING] = Utils::itoa(socket);
        } else if (!name.compare(WIFU_BIND_NAME)) {
            int return_val = 1;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_LISTEN_NAME)) {
            int return_val = 1;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_ACCEPT_NAME)) {
            int return_val = 1;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_SENDTO_NAME)) {
            int return_val = 1;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_RECVFROM_NAME)) {


        } else if (!name.compare(WIFU_CONNECT_NAME)) {
            int return_val = 0;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        }

        string response_message = QueryStringParser::create(name, response);
        send_to(m[FILE_STRING], response_message);
    }

private:

};

#endif	/* WIFUENDBACKENDLIBRARY_H */

