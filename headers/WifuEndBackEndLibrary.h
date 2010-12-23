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
#include "QueryStringParser.h"
#include "Socket.h"
#include "SocketCollection.h"
#include "ProtocolManager.h"

/**
 * Translates string messages received from the front-end library into Event objects
 * and enqueues them to the dispatcher (and visa-versa).
 *
 *
 */
class WifuEndBackEndLibrary : public LocalSocketFullDuplex, public Module {
public:

    static WifuEndBackEndLibrary& instance() {
        static WifuEndBackEndLibrary instance_;
        return instance_;
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

            int domain = atoi(m[DOMAIN_STRING].c_str());
            int type = atoi(m[TYPE_STRING].c_str());
            int protocol = atoi(m[PROTOCOL_STRING].c_str());

            int socket_id = -1;

            if (ProtocolManager::instance().is_supported(domain, type, protocol)) {
                Socket* socket = new Socket(domain, type, protocol);
                SocketCollection::instance().push(socket);
                socket_id = socket->get_socket();
                
            }
            response[SOCKET_STRING] = Utils::itoa(socket_id);

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
        } else if (!name.compare(WIFU_GETSOCKOPT_NAME)) {
            int return_val = SO_BINDTODEVICE;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_SETSOCKOPT_NAME)) {
            int return_val = 0;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        }

        // TODO: May not always want to respond immediately
        // TODO: We may need to wait for a response from the internal system
        string response_message = QueryStringParser::create(name, response);
        send_to(m[FILE_STRING], response_message);
    }

private:

    WifuEndBackEndLibrary() : LocalSocketFullDuplex("WS"), Module() {

    }

};

#endif	/* WIFUENDBACKENDLIBRARY_H */

