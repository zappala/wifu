#include "WifuEndBackEndLibrary.h"

WifuEndBackEndLibrary& WifuEndBackEndLibrary::instance() {
    static WifuEndBackEndLibrary instance_;
    return instance_;
}

WifuEndBackEndLibrary::~WifuEndBackEndLibrary() {

//    log_INFORMATIONAL("recv_events_size: ", pantheios::i(receive_events_.size()), " recv_response_events_size: ", pantheios::i(recv_response_events_.size()), " recv_response_sizes_size: ", pantheios::i(recv_response_sizes_.size()));
//    log_INFORMATIONAL("send_events_size: ", pantheios::i(send_events_.size()), " send_response_events_size: ", pantheios::i(send_response_events_.size()), " send_response_sizes_size: ", pantheios::i(send_response_sizes_.size()));
//
//    while (!recv_response_events_.empty()) {
//
//        int size = 1000;
//        char start[size];
//        char end[size];
//        char size_i[size];
//        memset(start, 0, size);
//        memset(end, 0, size);
//        memset(size_i, 0, size);
//
//        sprintf(start, "%llu", receive_events_.front());
//        sprintf(end, "%llu", recv_response_events_.front());
//        sprintf(size_i, "%u", recv_response_sizes_.front());
//
//        basic_string<PAN_CHAR_T> start_s(start);
//        basic_string<PAN_CHAR_T> end_s(end);
//        basic_string<PAN_CHAR_T> size_s(size_i);
//
//        log_INFORMATIONAL("recv_backend ", start_s, " ", end_s, " ", size_s);
//
//        receive_events_.pop_front();
//        recv_response_events_.pop_front();
//        recv_response_sizes_.pop_front();
//    }
//
//    while (!send_response_events_.empty()) {
//
//        int size = 1000;
//        char start[size];
//        char end[size];
//        char size_i[size];
//        memset(start, 0, size);
//        memset(end, 0, size);
//        memset(size_i, 0, size);
//
//        sprintf(start, "%llu", send_events_.front());
//        sprintf(end, "%llu", send_response_events_.front());
//        sprintf(size_i, "%u", send_response_sizes_.front());
//
//        basic_string<PAN_CHAR_T> start_s(start);
//        basic_string<PAN_CHAR_T> end_s(end);
//        basic_string<PAN_CHAR_T> size_s(size_i);
//
//        log_INFORMATIONAL("send_backend ", start_s, " ", end_s, " ", size_s);
//
//        send_events_.pop_front();
//        send_response_events_.pop_front();
//        send_response_sizes_.pop_front();
//    }
}

void WifuEndBackEndLibrary::receive(unsigned char* message, int length, u_int64_t& receive_time) {
    struct GenericMessage* gm = (struct GenericMessage*) message;

    LibraryEvent* e = NULL;
    Socket* socket = SocketCollection::instance().get_by_id(gm->fd);
    //    cout << "Message Type: " << gm->message_type << endl;

    switch (gm->message_type) {
        case WIFU_RECVFROM:
        case WIFU_RECV:
//            receive_events_.push_back(receive_time);
            e = ObjectPool<ReceiveEvent>::instance().get();
            break;

        case WIFU_SENDTO:
        case WIFU_SEND:
//            send_events_.push_back(receive_time);
            e = ObjectPool<SendEvent>::instance().get();
            break;

        case WIFU_SOCKET:
        {
            struct SocketMessage* sm = (struct SocketMessage*) message;
            if (ProtocolManager::instance().is_supported(sm->domain, sm->type, sm->protocol)) {
                e = ObjectPool<SocketEvent>::instance().get();
                socket = new Socket(sm->domain, sm->type, sm->protocol);
                SocketCollection::instance().push(socket);
            } else {
                ResponseEvent* response = ObjectPool<ResponseEvent>::instance().get();
                response->set_message_type(sm->message_type);
                response->set_fd(sm->fd);
                response->set_return_value(-1);
                response->set_errno(EPROTONOSUPPORT);
                response->set_default_length();
                response->set_destination(&(sm->source));
                u_int64_t time;
                send_to(response->get_destination(), response->get_buffer(), response->get_length(), &time);
                ObjectPool<ResponseEvent>::instance().release(response);
                return;
            }
            break;
        }

        case WIFU_BIND:
            e = ObjectPool<BindEvent>::instance().get();
            break;

        case WIFU_LISTEN:
            e = ObjectPool<ListenEvent>::instance().get();
            break;

        case WIFU_ACCEPT:
            e = ObjectPool<AcceptEvent>::instance().get();
            break;

        case WIFU_CONNECT:
            e = ObjectPool<ConnectEvent>::instance().get();
            break;

        case WIFU_GETSOCKOPT:
            e = ObjectPool<GetSocketOptionEvent>::instance().get();
            break;

        case WIFU_SETSOCKOPT:
            e = ObjectPool<SetSocketOptionEvent>::instance().get();
            break;

        case WIFU_CLOSE:
            e = ObjectPool<CloseEvent>::instance().get();
            break;

        default:
            throw WiFuException("Unknown message type");
    }

    assert(socket);


    if (e) {

        event_map_[socket->get_socket_id()] = e;
        //        memset(e->get_buffer(), 0, UNIX_SOCKET_MAX_BUFFER_SIZE);
        e->set_socket(socket);
        e->save_buffer(message, length);
        dispatch(e);
    }

}

void WifuEndBackEndLibrary::imodule_library_response(Event* e) {

    ResponseEvent* event = (ResponseEvent*) e;
    //    cout << "WifuEndBackEndLibrary::imodule_library_response(), writing to: " << event->get_destination()->sun_path << endl;

    //    cout << "Back end library return value for message " << event->get_response()->message_type << ": " << event->get_response()->return_value << endl;
//    u_int64_t time;

    event_map_iterator_ = event_map_.find(event->get_socket()->get_socket_id());
    assert(event_map_iterator_ != event_map_.end());

    LibraryEvent* original_event = event_map_iterator_->second;

    ssize_t sent = send_to(event->get_destination(), event->get_buffer(), event->get_length(), 0);
    assert(sent == event->get_length());

    switch (original_event->get_message_type()) {
        case WIFU_RECVFROM:
        case WIFU_RECV:
//            recv_response_events_.push_back(time);
//            recv_response_sizes_.push_back(event->get_response()->return_value);
            ObjectPool<ReceiveEvent>::instance().release((ReceiveEvent*) original_event);
            break;
        case WIFU_SENDTO:
        case WIFU_SEND:
//            send_response_events_.push_back(time);
//            send_response_sizes_.push_back(event->get_response()->return_value);
            ObjectPool<SendEvent>::instance().release((SendEvent*) original_event);
            break;
        case WIFU_SOCKET:
            ObjectPool<SocketEvent>::instance().release((SocketEvent*) original_event);
            break;
        case WIFU_BIND:
            ObjectPool<BindEvent>::instance().release((BindEvent*) original_event);
            break;
        case WIFU_LISTEN:
            ObjectPool<ListenEvent>::instance().release((ListenEvent*) original_event);
            break;
        case WIFU_ACCEPT:
            ObjectPool<AcceptEvent>::instance().release((AcceptEvent*) original_event);
            break;
        case WIFU_CONNECT:
            ObjectPool<ConnectEvent>::instance().release((ConnectEvent*) original_event);
            break;
        case WIFU_GETSOCKOPT:
            ObjectPool<GetSocketOptionEvent>::instance().release((GetSocketOptionEvent*) original_event);
            break;
        case WIFU_SETSOCKOPT:
            ObjectPool<SetSocketOptionEvent>::instance().release((SetSocketOptionEvent*) original_event);
            break;
        case WIFU_CLOSE:
            ObjectPool<CloseEvent>::instance().release((CloseEvent*) original_event);
            break;
        default:
            throw WiFuException("Unknown message type");
    }

    ObjectPool<ResponseEvent>::instance().release(event);
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
