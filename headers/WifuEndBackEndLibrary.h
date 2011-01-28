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
#include "events/BindEvent.h"
#include "events/ResponseEvent.h"
#include "events/ListenEvent.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "UDPInterface.h"
#include "PortManager.h"

#define SOURCE_ADDRESS UDPInterface::instance().get_bound_ip_address()

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

        string name = m[NAME_STRING];
        string s = m[SOCKET_STRING];
        Socket* socket = SocketCollection::instance().get_by_id(atoi(s.c_str()));

        if (!name.compare(WIFU_SOCKET_NAME)) {

            int domain = atoi(m[DOMAIN_STRING].c_str());
            int type = atoi(m[TYPE_STRING].c_str());
            int protocol = atoi(m[PROTOCOL_STRING].c_str());

            if (ProtocolManager::instance().is_supported(domain, type, protocol)) {
                Socket* socket = new Socket(domain, type, protocol, new AddressPort(SOURCE_ADDRESS, PortManager::instance().get()));
                SocketCollection::instance().push(socket);

                dispatch(new SocketEvent(message, getFile(), socket));
                return;

            } else {
                map<string, string> response;
                response[SOCKET_STRING] = s;
                response[FILE_STRING] = getFile();
                response[SOCKET_STRING] = Utils::itoa(-1);
                response[ERRNO] = Utils::itoa(EPROTONOSUPPORT);
                // TODO: May not always want to respond immediately
                // TODO: We may need to wait for a response from the internal system
                string response_message = QueryStringParser::create(name, response);
                send_to(m[FILE_STRING], response_message);
            }


        } else if (!name.compare(WIFU_BIND_NAME)) {

            dispatch(new BindEvent(message, getFile(), socket));
            return;

        } else if (!name.compare(WIFU_LISTEN_NAME)) {

            dispatch(new ListenEvent(message, getFile(), socket));
            return;
            
        } else if (!name.compare(WIFU_ACCEPT_NAME)) {
            dispatch(new AcceptEvent(message, getFile(), socket));
            return;
            
        } else if (!name.compare(WIFU_SENDTO_NAME)) {
            int return_val = 1;
            //            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_RECVFROM_NAME)) {


        } else if (!name.compare(WIFU_CONNECT_NAME)) {
            dispatch(new ConnectEvent(message, getFile(), socket));
            return;

        } else if (!name.compare(WIFU_GETSOCKOPT_NAME)) {
            int return_val = SO_BINDTODEVICE;
            //            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_SETSOCKOPT_NAME)) {
            int return_val = 0;
            //            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        }
    }

    void library_response(Event* e) {
        ResponseEvent* event = (ResponseEvent*) e;
        event->put(FILE_STRING, getFile());
        string file = event->get_write_file();
        string response = event->get_response();
//        cout << "Response: " << response << endl;
        send_to(file, response);
    }




private:

    WifuEndBackEndLibrary() : LocalSocketFullDuplex("/tmp/WS"), Module() {

    }

};

#endif	/* WIFUENDBACKENDLIBRARY_H */

