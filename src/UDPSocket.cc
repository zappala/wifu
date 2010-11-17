/* 
 * File:   UDPSocket.cc
 * Author: rbuck
 * 
 * Created on June 15, 2010, 3:22 PM
 */

#include "UDPSocket.h"


UDPSocket::UDPSocket() {
    createSocket();
    sem_ = new Semaphore();
    sem_->init(0);

}

UDPSocket::~UDPSocket() {
    delete sem_;
}

int UDPSocket::createSocket() {
    if ((this->sock_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket");
        exit(EXIT_FAILURE);
    }

    return getSocket();
}

int UDPSocket::closeSocket() {
    return close(getSocket());
}

int UDPSocket::getSocket() const {
    return this->sock_;
}

void UDPSocket::makeNonBlocking() {
    long arg;
    if ((arg = fcntl(getSocket(), F_GETFL, NULL)) < 0) {
        fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    arg |= O_NONBLOCK;
    if (fcntl(getSocket(), F_SETFL, arg) < 0) {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

size_t UDPSocket::send(AddressPort& ap, string& message) {
    return send(ap.get_network_struct_ptr(), (unsigned char*)message.c_str(), message.length());
}

size_t UDPSocket::send(AddressPort& ap, const unsigned char* message, socklen_t length) {
    return send(ap.get_network_struct_ptr(), message, length);
}

size_t UDPSocket::send(struct sockaddr_in* address, const unsigned char* message, socklen_t length) {

    size_t count = sendto(getSocket(), message, length, 0, (const struct sockaddr*)address, sizeof (struct sockaddr));

    if (count < 0) {
        perror("error in sending");
    }

    return count;
}

void UDPSocket::bind_socket(const string& host, int port) {

    // use DNS to get host name
    struct hostent * host_entry = gethostbyname(host.c_str());
    if (!host_entry) {
        perror("No such host name");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in sin;
    memset(&sin, 0, sizeof (sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    if (host == "any") {
        sin.sin_addr.s_addr = INADDR_ANY;
    } else {
        memcpy(&sin.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
    }

    if (bind(getSocket(), (const struct sockaddr *) & sin, sizeof (sin)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void UDPSocket::set_receive_timeout(int sec) {
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = sec;
    if (setsockopt(getSocket(), SOL_SOCKET, SO_RCVTIMEO, (char *) & tv, sizeof tv)) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void UDPSocket::receive(UDPSocketCallback* callback) {
    struct socket_thread_data data;
    data.callback = callback;
    data.socket = getSocket();
    data.sem = sem_;

    if (pthread_create(&receive_thread_, NULL, &receive_handler, &data) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }

    sem_->wait();
}

void * receive_handler(void * arg) {
    struct socket_thread_data * data = (socket_thread_data *) arg;
    int socket = data->socket;
    UDPSocketCallback * callback = data->callback;


    unsigned char buf[BUF_LENGTH];
    struct sockaddr_in address;
    socklen_t length = sizeof (struct sockaddr_in);

    data->sem->post();

    while (1) {
        /* get a packet */
        memset(buf, 0, BUF_LENGTH);
        //ssize_t num = recv(socket, &buf, BUF_LENGTH, 0);
        ssize_t num = recvfrom(socket, &buf, BUF_LENGTH, 0, (struct sockaddr*) & address, &length);
        
        AddressPort ap(&address);

        if (num < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN) {
                break;
            }
            printf("\n");
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        callback->receive(ap, buf, num);
    }
}

