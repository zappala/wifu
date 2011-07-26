#include "LocalSocketSender.h"

LocalSocketSender::LocalSocketSender() {
    init();
}

LocalSocketSender::~LocalSocketSender() {
    map<gcstring, struct sockaddr_un*, std::less<gcstring>, gc_allocator<std::pair<gcstring, struct sockaddr_un*> > >::iterator itr;

    // show content:
    for (itr = destinations_.begin(); itr != destinations_.end(); itr++) {
        delete itr->second;
    }

    close(socket_);
}


// TODO: do we need to protect destinations_ with a semaphore?
ssize_t LocalSocketSender::send_to(gcstring& socket_file, gcstring& message) {
    struct sockaddr_un* destination = destinations_[socket_file];
    if (!destination) {
        destination = create_socket(socket_file);
    }
    return sendto(socket_, message.data(), message.size(), 0, (const struct sockaddr*) destination, SUN_LEN(destination));
}

struct sockaddr_un* LocalSocketSender::create_socket(gcstring& socket_file) {
    struct sockaddr_un* destination = new struct sockaddr_un;

    // Setup socket address structure
    memset(destination, 0, sizeof (struct sockaddr_un));
    destination->sun_family = AF_LOCAL;
    strcpy(destination->sun_path, socket_file.c_str());


    assert(destination->sun_family == AF_LOCAL);
    assert(socket_file == gcstring(destination->sun_path));


    destinations_[socket_file] = destination;
    return destination;
}

void LocalSocketSender::init() {
    socket_ = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (!socket_) {
        perror("Error Creating Socket");
        exit(-1);
    }

    socklen_t optval = MAX_BUFFER_SIZE;
    int value = setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, &optval, sizeof (optval));
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}
