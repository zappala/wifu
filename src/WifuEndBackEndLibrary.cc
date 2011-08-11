#include "WifuEndBackEndLibrary.h"
#include "MessageStructDefinitions.h"

WifuEndBackEndLibrary& WifuEndBackEndLibrary::instance() {
    static WifuEndBackEndLibrary instance_;
    return instance_;
}

WifuEndBackEndLibrary::~WifuEndBackEndLibrary() {

    log_INFORMATIONAL("recv_events_size: ", pantheios::i(receive_events_.size()), " recv_response_events_size: ", pantheios::i(recv_response_events_.size()), " recv_response_sizes_size: ", pantheios::i(recv_response_sizes_.size()));
    log_INFORMATIONAL("send_events_size: ", pantheios::i(send_events_.size()), " send_response_events_size: ", pantheios::i(send_response_events_.size()), " send_response_sizes_size: ", pantheios::i(send_response_sizes_.size()));

    while (!recv_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);

        sprintf(start, "%llu", receive_events_.front());
        sprintf(end, "%llu", recv_response_events_.front());
        sprintf(size_i, "%du", recv_response_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);

        log_INFORMATIONAL("recv_backend ", start_s, " ", end_s, " ", size_s);

        receive_events_.pop_front();
        recv_response_events_.pop_front();
        recv_response_sizes_.pop_front();
    }

    while (!send_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);

        sprintf(start, "%llu", send_events_.front());
        sprintf(end, "%llu", send_response_events_.front());
        sprintf(size_i, "%du", send_response_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);

        log_INFORMATIONAL("send_backend ", start_s, " ", end_s, " ", size_s);

        send_events_.pop_front();
        send_response_events_.pop_front();
        send_response_sizes_.pop_front();
    }


}

void WifuEndBackEndLibrary::receive(gcstring& message, u_int64_t& receive_time) {
    // TODO: this method is way too long (and will likely get bigger)
    // TODO: refactor this method to use objects as much as possible

    //                cout << "WifuEndBackEndLibrary::receive(), message: " << message << endl;

//    gcstring_map m;
//    try {
//        QueryStringParser::parse(message, m);
//    } catch (WiFuException e) {
//        return;
//    }
//
//    gcstring& name = m[NAME_STRING];
//    gcstring& s = m[SOCKET_STRING];
//    int socket_int = atoi(s.c_str());
//    //assert(sockInt != 0);
//    Socket* socket = SocketCollection::instance().get_by_id(socket_int);
//
//    if (!name.compare(WIFU_RECVFROM_NAME)) {
//        //        cout << Utils::get_current_time_microseconds_32() << " WifuEndBackEndLibrary::receive(), ReceiveEvent to be dispatched" << endl;
//        //log_INFORMATIONAL("recv_event ");
//        receive_events_.push_back(receive_time);
//        dispatch(new ReceiveEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_SENDTO_NAME)) {
//        send_events_.push_back(receive_time);
//        dispatch(new SendEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_BIND_NAME)) {
//        dispatch(new BindEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_LISTEN_NAME)) {
//        dispatch(new ListenEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_ACCEPT_NAME)) {
//        dispatch(new AcceptEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_CONNECT_NAME)) {
//        dispatch(new ConnectEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_GETSOCKOPT_NAME)) {
//        dispatch(new GetSocketOptionEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_SETSOCKOPT_NAME)) {
//        dispatch(new SetSocketOptionEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_CLOSE_NAME)) {
//        dispatch(new CloseEvent(m, get_file(), socket));
//        return;
//
//    } else if (!name.compare(WIFU_SOCKET_NAME)) {
//
//
//        int domain = atoi(m[DOMAIN_STRING].c_str());
//        int type = atoi(m[TYPE_STRING].c_str());
//        int protocol = atoi(m[PROTOCOL_STRING].c_str());
//
//        if (ProtocolManager::instance().is_supported(domain, type, protocol)) {
//            Socket* socket = new Socket(domain, type, protocol);
//            SocketCollection::instance().push(socket);
//
//            dispatch(new SocketEvent(m, get_file(), socket));
//            return;
//
//        } else {
//            gcstring_map response;
//            response[SOCKET_STRING] = s;
//            response[FILE_STRING] = get_file();
//            response[SOCKET_STRING] = Utils::itoa(-1);
//            response[ERRNO] = Utils::itoa(EPROTONOSUPPORT);
//            // TODO: May not always want to respond immediately
//            // TODO: We may need to wait for a response from the internal system
//            gcstring response_message;
//            QueryStringParser::create(name, response, response_message);
//            u_int64_t time;
//            send_to(m[FILE_STRING], response_message, &time);
//        }
//    }
}

void WifuEndBackEndLibrary::receive(unsigned char* message, int length, u_int64_t& receive_time) {
    struct GenericMessage* gm = reinterpret_cast<struct GenericMessage*> (message);

    LibraryEvent* e = NULL;
    Socket* socket = SocketCollection::instance().get_by_id(gm->fd);
    
    switch (gm->message_type) {
        case WIFU_SOCKET:
        {
            e = ObjectPool<SocketEvent>::instance().get();
            struct SocketMessage* sm = reinterpret_cast<struct SocketMessage*> (message);

            if (ProtocolManager::instance().is_supported(sm->domain, sm->type, sm->protocol)) {
                socket = new Socket(sm->domain, sm->type, sm->protocol);
                SocketCollection::instance().push(socket);
            } else {
                ResponseEvent* response = ObjectPool<ResponseEvent>::instance().get();
                response->set_default_length();
                response->set_fd(sm->fd);
                response->set_return_value(-1);
                response->set_errno(EPROTONOSUPPORT);
                response->set_message_type(sm->message_type);
                u_int64_t time;
                send_to(&(sm->source), response->get_buffer(), response->get_length(), &time);
                ObjectPool<ResponseEvent>::instance().release(response);
                return;
            }
            cout << "WIFU_SOCKET: " << endl;
            break;
        }            

        case WIFU_BIND:
            cout << "WIFU_BIND" << endl;
            e = ObjectPool<BindEvent>::instance().get();
            break;

        case WIFU_LISTEN:
            cout << "WIFU_LISTEN" << endl;
            e = ObjectPool<ListenEvent>::instance().get();
            break;

        case WIFU_ACCEPT:
            cout << "WIFU_ACCEPT" << endl;
            e = ObjectPool<AcceptEvent>::instance().get();
            break;

        case WIFU_SENDTO:
        case WIFU_SEND:
            cout << "WIFU_SEND(TO)" << endl;
            e = ObjectPool<SendEvent>::instance().get();
            break;

        case WIFU_RECVFROM:
        case WIFU_RECV:
            cout << "WIFU_RECV(FROM)" << endl;
            e = ObjectPool<ReceiveEvent>::instance().get();
            break;

        case WIFU_CONNECT:
            cout << "WIFU_CONNECT" << endl;
            e = ObjectPool<ConnectEvent>::instance().get();
            break;

        case WIFU_GETSOCKOPT:
            cout << "WIFU_GETSOCKOPT" << endl;
            e = ObjectPool<GetSocketOptionEvent>::instance().get();
            break;

        case WIFU_SETSOCKOPT:
            cout << "WIFU_SETSOCKOPT" << endl;
            e = ObjectPool<SetSocketOptionEvent>::instance().get();
            break;

        case WIFU_CLOSE:
            cout << "WIFU_CLOSE" << endl;
            e = ObjectPool<CloseEvent>::instance().get();
            break;

        default:
            return;
            //throw WiFuException("Unknown message type");
    }

    //TODO: insert socket


    if(e) {
        e->set_socket(socket);
        e->save_buffer(message, length);
        dispatch(e);
    }

}

void WifuEndBackEndLibrary::imodule_library_response(Event* e) {
    ResponseEvent* event = (ResponseEvent*) e;
    u_int64_t time;
    send_to(event->get_destination(), event->get_buffer(), event->get_length(), &time);
    
    if (event->get_response()->message_type == WIFU_RECVFROM) {
        //cout << Utils::get_current_time_microseconds_32() << " WifuEndBackEndLibrary::imodule_library_response()" << endl;
        //log_INFORMATIONAL("recv_response_event ", (pan_uint64_t) Utils::get_current_time_microseconds_64());
        recv_response_events_.push_back(time);
        recv_response_sizes_.push_back(event->get_response()->return_value);
    } else if (event->get_response()->message_type == WIFU_SENDTO) {
        //cout << Utils::get_current_time_microseconds_32() << " WifuEndBackEndLibrary::imodule_library_response()" << endl;
        //log_INFORMATIONAL("recv_response_event ", (pan_uint64_t) Utils::get_current_time_microseconds_64());
        send_response_events_.push_back(time);
        send_response_sizes_.push_back(event->get_response()->return_value);
    }

    ObjectPool<ResponseEvent>::instance().release(event);

    // TODO: release the Event

}

WifuEndBackEndLibrary::WifuEndBackEndLibrary() : LocalSocketFullDuplex("/tmp/WS"), Module() {
    ObjectPool<SocketEvent>::instance();
    ObjectPool<BindEvent>::instance();
    ObjectPool<ListenEvent>::instance();
    ObjectPool<AcceptEvent>::instance();
    ObjectPool<SendEvent>::instance();
    ObjectPool<ReceiveEvent>::instance();
    ObjectPool<ConnectEvent>::instance();
    ObjectPool<GetSocketOptionEvent>::instance();
    ObjectPool<SetSocketOptionEvent>::instance();
    ObjectPool<CloseEvent>::instance();

    ObjectPool<ResponseEvent>::instance();

    log_INFORMATIONAL("WiFuBackEndLibrary Created");
}
