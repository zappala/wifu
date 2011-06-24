#include "WifuEndBackEndLibrary.h"

WifuEndBackEndLibrary& WifuEndBackEndLibrary::instance() {
    static WifuEndBackEndLibrary instance_;
    return instance_;
}

WifuEndBackEndLibrary::~WifuEndBackEndLibrary() {
}

void WifuEndBackEndLibrary::receive(string& message) {
    // TODO: this method is way too long (and will likely get bigger)
    // TODO: refactor this method to use objects as much as possible

    //        cout << "WifuEndBackEndLibrary::receive(), message: " << message << endl;


    map<string, string> m;
    QueryStringParser::parse(message, m);

    string name = m[NAME_STRING];
    string s = m[SOCKET_STRING];
    int socket_int = atoi(s.c_str());
    //assert(sockInt != 0);
    Socket* socket = SocketCollection::instance().get_by_id(socket_int);

    if (!name.compare(WIFU_SOCKET_NAME)) {

        int domain = atoi(m[DOMAIN_STRING].c_str());
        int type = atoi(m[TYPE_STRING].c_str());
        int protocol = atoi(m[PROTOCOL_STRING].c_str());

        if (ProtocolManager::instance().is_supported(domain, type, protocol)) {
            Socket* socket = new Socket(domain, type, protocol);
            SocketCollection::instance().push(socket);

            dispatch(new SocketEvent(message, get_file(), socket));
            return;

        } else {
            map<string, string> response;
            response[SOCKET_STRING] = s;
            response[FILE_STRING] = get_file();
            response[SOCKET_STRING] = Utils::itoa(-1);
            response[ERRNO] = Utils::itoa(EPROTONOSUPPORT);
            // TODO: May not always want to respond immediately
            // TODO: We may need to wait for a response from the internal system
            string response_message = QueryStringParser::create(name, response);
            send_to(m[FILE_STRING], response_message);
        }


    } else if (!name.compare(WIFU_BIND_NAME)) {
        dispatch(new BindEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_LISTEN_NAME)) {
        dispatch(new ListenEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_ACCEPT_NAME)) {
        dispatch(new AcceptEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_SENDTO_NAME)) {
        dispatch(new SendEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_RECVFROM_NAME)) {
        dispatch(new ReceiveEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_CONNECT_NAME)) {
        dispatch(new ConnectEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_GETSOCKOPT_NAME)) {
        dispatch(new GetSocketOptionEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_SETSOCKOPT_NAME)) {
        dispatch(new SetSocketOptionEvent(message, get_file(), socket));
        return;

    } else if (!name.compare(WIFU_CLOSE_NAME)) {
        dispatch(new CloseEvent(message, get_file(), socket));
        return;
    }
}

void WifuEndBackEndLibrary::imodule_library_response(Event* e) {
    ResponseEvent* event = (ResponseEvent*) e;
    event->put(FILE_STRING, get_file());
    string file = event->get_write_file();
    string response = event->get_response();
//    cout << "Response: " << response << endl;
    send_to(file, response);
}

WifuEndBackEndLibrary::WifuEndBackEndLibrary() : LocalSocketFullDuplex("/tmp/WS"), Module() {
}
