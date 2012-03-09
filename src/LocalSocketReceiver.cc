#include "LocalSocketReceiver.h"
#include "Utils.h"

LocalSocketReceiver::LocalSocketReceiver(gcstring& file, LocalSocketReceiverCallback* callback) : file_(file), callback_(callback) {
    init();
}

LocalSocketReceiver::LocalSocketReceiver(const char* file, LocalSocketReceiverCallback* callback) : file_(file), callback_(callback) {
    init();
}

LocalSocketReceiver::~LocalSocketReceiver() {
    close(socket_);
    unlink(file_.c_str());
    pthread_cancel(thread_);
}

int LocalSocketReceiver::get_socket() {
    return socket_;
}

gcstring& LocalSocketReceiver::get_file() {
    return file_;
}

LocalSocketReceiverCallback* LocalSocketReceiver::get_callback() const {
    return callback_;
}

struct sockaddr_un* LocalSocketReceiver::get_address() {
    return &server_;
}

void LocalSocketReceiver::init(void) {

    sem_.init(1);

    

    // setup socket address structure
    memset(&server_, 0, sizeof (server_));
    server_.sun_family = AF_LOCAL;
    strcpy(server_.sun_path, file_.c_str());

    // create socket
    socket_ = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (!get_socket()) {
        perror("Error creating Unix socket");
        exit(-1);
    }
    unlink(file_.c_str());

    int optval = 1;
    int value = setsockopt(get_socket(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    optval = UNIX_SOCKET_MAX_BUFFER_SIZE;
    value = setsockopt(get_socket(), SOL_SOCKET, SO_RCVBUF, &optval, sizeof (optval));
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(get_socket(), (const struct sockaddr *) & server_, SUN_LEN(&server_)) < 0) {
        perror("Bind");
        exit(-1);
    }

    struct local_socket_receiver_obj obj;
    obj.sock = get_socket();
    obj.receiver = this;
    obj.sem.init(0);

    if (pthread_create(&thread_, NULL, &unix_receive_handler, &obj) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }
    obj.sem.wait();
}

void* unix_receive_handler(void* arg) {
    struct local_socket_receiver_obj* obj = (struct local_socket_receiver_obj*) arg;

    LocalSocketReceiverCallback* receiver = obj->receiver->get_callback();
    int socket = obj->sock;

    obj->sem.post();

    unsigned char buf[UNIX_SOCKET_MAX_BUFFER_SIZE];

    int nread;
    u_int64_t time;

    while (1) {
        nread = recv(socket, buf, UNIX_SOCKET_MAX_BUFFER_SIZE, 0);
        if (nread < 0) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else if (nread == 0) {
            // the socket is closed
            break;
        }

        receiver->receive(buf, nread, time);
    }
}

