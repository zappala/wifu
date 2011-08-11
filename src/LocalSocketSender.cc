#include "LocalSocketSender.h"
#include "Utils.h"

LocalSocketSender::LocalSocketSender() {
    init();
}

LocalSocketSender::~LocalSocketSender() {
    close(socket_);
}

ssize_t LocalSocketSender::send_to(struct sockaddr_un* destination, void* buffer, size_t length, u_int64_t* send_time) {
    *send_time = Utils::get_current_time_microseconds_64();
    return sendto(socket_, buffer, length, 0, reinterpret_cast<const struct sockaddr*>(destination), SUN_LEN(destination));
}

void LocalSocketSender::init() {
    socket_ = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (!socket_) {
        perror("Error Creating Socket");
        exit(-1);
    }

    socklen_t optval = UNIX_SOCKET_MAX_BUFFER_SIZE;
    int value = setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, &optval, sizeof (optval));
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}
