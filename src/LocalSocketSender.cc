#include "LocalSocketSender.h"

LocalSocketSender::LocalSocketSender() {

}

LocalSocketSender::~LocalSocketSender() {

}

void LocalSocketSender::send_to(string & socket_file, string & message) {
    if (!sockets_[socket_file]) {
        create_socket(socket_file);
    }
    send_to(sockets_[socket_file], message);
}

void LocalSocketSender::send_to(int & socket, string & message) {
    send(socket, message.c_str(), message.length(), 0);
}

void LocalSocketSender::create_socket(string & socket_file) {
    struct sockaddr_un server;

    // Setup socket address structure
    memset(&server, 0, sizeof (server));
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, socket_file.c_str());


    // Create socket
    int s = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (!s) {
        perror("socket");
        exit(-1);
    }

    // Connect to server
    if (connect(s, (const struct sockaddr *) & server, sizeof (server)) < 0) {
        perror("connect");
        exit(-1);
    }

    sockets_[socket_file] = s;
}
