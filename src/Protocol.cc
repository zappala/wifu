#include "Protocol.h"
#include "events/ConnectionInitiatedEvent.h"
#include "events/ReceiveEvent.h"

Protocol::Protocol(int protocol) : Module(), protocol_(protocol) {

}

Protocol::~Protocol() {

}

void Protocol::library_socket(Event* e) {
    SocketEvent* event = (SocketEvent*) e;

    Socket* s = event->get_socket();
    // This is to filter out bind events which do not correspond to this Protocol
    // TODO: this will not (likely) scale well
    if (s->get_protocol() != protocol_) {
        return;
    }

    sockets_.insert(s);
    socket(event);

    ResponseEvent* response = new ResponseEvent(s, event->get_name(), event->get_map()[FILE_STRING]);
    response->put(ERRNO, Utils::itoa(0));
    response->put(RETURN_VALUE_STRING, Utils::itoa(0));

    dispatch(response);
}

void Protocol::library_bind(Event* e) {
    BindEvent* event = (BindEvent*) e;

    int error = 0;
    int return_val = -1;
    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: may not need this check, it is done on the front end.
    // TODO: I'm leaving it for now, just to be safe.
    if (socket != NULL) {

        u_int16_t port = atoi(event->get_map()[PORT_STRING].c_str());
        string address = event->get_map()[ADDRESS_STRING];
        AddressPort* local = new AddressPort(address, port);

        // TODO: Check possible errors
        AlreadyBoundToAddressPortVisitor v(local);
        SocketCollection::instance().accept(&v);

        if (!v.is_bound()) {
            socket->set_local_address_port(local);
            bind(event);
            return_val = 0;
        } else {
            error = EINVAL;
        }

    } else {
        error = EBADF;
    }

    ResponseEvent* response = new ResponseEvent(socket, event->get_name(), event->get_map()[FILE_STRING]);
    response->put(ERRNO, Utils::itoa(error));
    response->put(RETURN_VALUE_STRING, Utils::itoa(return_val));

    dispatch(response);
}

void Protocol::library_listen(Event* e) {
    ListenEvent* event = (ListenEvent*) e;

    Socket* socket = event->get_socket();
    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: Do something with this back log
    int back_log = event->get_back_log();

    int error = 0;
    int return_val = 0;

    // TODO: do we need to check the address and port or just the port?
    AlreadyListeningOnSamePortVisitor v(socket->get_local_address_port()->get_port());
    SocketCollection::instance().accept(&v);

    // TODO: check EOPNOTSUPP:
    // The socket is not of a type that supports  the  listen()  operation.
    // see man 2 listen

    if (v.is_listening()) {
        error = EADDRINUSE;
        return_val = -1;
    } else {
        listen(event);
    }

    ResponseEvent* response = new ResponseEvent(socket, event->get_name(), event->get_map()[FILE_STRING]);
    response->put(ERRNO, Utils::itoa(error));
    response->put(RETURN_VALUE_STRING, Utils::itoa(return_val));

    // TODO: we may not need this if we are pushing things into the FSMs
    socket->make_passive();
    dispatch(response);
}

void Protocol::library_connect(Event* e) {
    ConnectEvent* event = (ConnectEvent*) e;

    Socket* socket = event->get_socket();
    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: Error check

    //    cout << "In library connect" << endl;
    connect(event);
}

void Protocol::library_accept(Event* e) {
    cout << "Protocol::library_accept(), Event socket: " << e->get_socket() << endl;
    AcceptEvent* event = (AcceptEvent*) e;

    Socket* socket = event->get_socket();
    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: Error check
    accept(event);
}

void Protocol::library_receive(Event* e) {
    //    cout << "Protocol::library_receive()" << endl;
    ReceiveEvent* event = (ReceiveEvent*) e;

    Socket* s = event->get_socket();

    if (!icontext_can_receive(s)) {
        // TODO: respond with error
        return;
    }


    receive_from(event);
}

void Protocol::library_send(Event* e) {
    //    cout << "Protocol::library_send()" << endl;
    SendEvent* event = (SendEvent*) e;

    if(!icontext_can_send(e->get_socket())) {
        // TODO: respond with error
        return;
    }

    // call contexts
    send_to(event);
}

void Protocol::library_close(Event* e) {
    CloseEvent* event = (CloseEvent*)e;
    if(!sockets_.contains(event->get_socket())) {
        // TODO: return an error?
        return;
    }
    icontext_close(event);
}

void Protocol::send(Event* e) {
    //    cout << "Protocol::send()" << endl;
    SendPacketEvent* event = (SendPacketEvent*) e;

    Socket* socket = event->get_socket();
    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: Error check
    send_packet(event);
}

void Protocol::network_receive(Event* e) {
    //    cout << "Protocol::network_receive()" << endl;

    NetworkReceivePacketEvent* event = (NetworkReceivePacketEvent*) e;

    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    // TODO: Error check
    // We will potentially save data before we are connected; however, we won't pass data to the application until we are connected.
    receive_packet(event);
}

void Protocol::connection_established(Event* e) {
    cout << "Protocol::connection_established()" << endl;
    // TODO: a lot of this code is the same as in library_socket, refactor later
    ConnectionEstablishedEvent* event = (ConnectionEstablishedEvent*) e;

    Socket* socket = event->get_socket();

    // This is to filter out bind events which do not correspond to this Protocol
    // TODO: this will not (likely) scale well
    if (socket->get_protocol() != protocol_) {
        return;
    }

    Socket* new_socket = event->get_new_socket();

    new_connection_established(event);

    AcceptEvent* a_event = event->get_accept_event();

    ResponseEvent* response = new ResponseEvent(socket, a_event->get_name(), a_event->get_map()[FILE_STRING]);
    response->put(ERRNO, Utils::itoa(0));
    response->put(RETURN_VALUE_STRING, Utils::itoa(new_socket->get_socket_id()));
    response->put(ADDRESS_STRING, new_socket->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(new_socket->get_remote_address_port()->get_port()));

    dispatch(response);
}

void Protocol::connection_initiated(Event* e) {
    ConnectionInitiatedEvent* event = (ConnectionInitiatedEvent*) e;
    Socket* listening_socket = event->get_socket();
    Socket* new_socket = event->get_new_socket();

    sockets_.insert(new_socket);

    // TODO: Error Check: socket(s)

    new_conneciton_initiated(event);
}

void Protocol::timer_fired(Event* e) {
    TimerFiredEvent* event = (TimerFiredEvent*) e;
    Socket* socket = event->get_socket();


    if (!sockets_.contains(socket)) {
        return;
    }

    timer_fired_event(event);
}

void Protocol::resend(Event* e) {
    ResendPacketEvent* event = (ResendPacketEvent*) e;
    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    resend_packet(event);
}

void Protocol::send_buffer_not_empty(Event* e) {
    SendBufferNotEmptyEvent* event = (SendBufferNotEmptyEvent*) e;
    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    icontext_send_buffer_not_empty(event);
}

void Protocol::send_buffer_not_full(Event* e) {
    SendBufferNotFullEvent* event = (SendBufferNotFullEvent*) e;
    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    icontext_send_buffer_not_full(event);
}

void Protocol::receive_buffer_not_empty(Event* e) {
    ReceiveBufferNotEmptyEvent* event = (ReceiveBufferNotEmptyEvent*) e;
    Socket* socket = event->get_socket();

    if (!sockets_.contains(socket)) {
        return;
    }

    icontext_receive_buffer_not_empty(event);
}