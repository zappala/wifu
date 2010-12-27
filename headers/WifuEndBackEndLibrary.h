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
#include "Utils.h"
#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "events/SocketEvent.h"

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

        // TODO: this method is way too long (and will likely get bigger)
        // TODO: refactor this method to use objects as much as possible

        map<string, string> m;
        QueryStringParser::parse(message, m);

        map<string, string> response;
        response[FILE_STRING] = getFile();
        string name = m[NAME_STRING];
        string s = m[SOCKET_STRING];
        response[SOCKET_STRING] = s;
        int socket_id = atoi(s.c_str());
        int error = 0; // No error by default

        if (!name.compare(WIFU_SOCKET_NAME)) {

            int domain = atoi(m[DOMAIN_STRING].c_str());
            int type = atoi(m[TYPE_STRING].c_str());
            int protocol = atoi(m[PROTOCOL_STRING].c_str());

            socket_id = -1;

            if (ProtocolManager::instance().is_supported(domain, type, protocol)) {
                // TODO: delete Sockets at some point (or use GC)
                Socket* socket = new Socket(domain, type, protocol);
                SocketCollection::instance().push(socket);
                socket_id = socket->get_socket();
                
                // TODO: figure out if we need to do this...
                Dispatcher::instance().enqueue(new SocketEvent(message, getFile()));

            } else {
                error = EPROTONOSUPPORT;
            }
            response[SOCKET_STRING] = Utils::itoa(socket_id);

        } else if (!name.compare(WIFU_BIND_NAME)) {
            int return_val = -1;

            Socket* socket = SocketCollection::instance().get_by_id(socket_id);
            if (socket != NULL) {

                u_int16_t port = atoi(m[PORT_STRING].c_str());
                AddressPort* local = new AddressPort(m[ADDRESS_STRING], port);

                // TODO: Check possible errors

                AlreadyBoundToAddressPortVisitor v(local);
                SocketCollection::instance().accept(&v);

                if (!v.is_bound()) {
                    socket->set_local_address_port(local);
                    return_val = 0;
                } else {
                    error = EINVAL;
                }

            } else {
                error = EBADF;
            }



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

        response[ERRNO] = Utils::itoa(error);
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

