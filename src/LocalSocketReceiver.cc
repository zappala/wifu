#include <typeinfo>

#include "LocalSocketReceiver.h"


LocalSocketReceiver::LocalSocketReceiver(string& file, LocalSocketReceiverCallback* callback) : file_(file), callback_(callback) {
    init();
}

LocalSocketReceiver::LocalSocketReceiver(const char* file, LocalSocketReceiverCallback* callback) : file_(file), callback_(callback) {
    init();
}

LocalSocketReceiver::~LocalSocketReceiver() {
    unlink(file_.c_str());    
}

void LocalSocketReceiver::kill_threads() {
    while (!ts_.is_empty()) {
        pair<pthread_t, int> item = ts_.pop();

        cout << "Closing socket: " << item.second << endl;
        close(item.second);

        cout << "Killing thread: " << item.first << endl;
        pthread_cancel(item.first);
    }
}

string & LocalSocketReceiver::getFile() {
    return file_;
}

void LocalSocketReceiver::recv(string & message) {
    sem_wait(&sem_);
    callback_->receive(message);
    sem_post(&sem_);
}

void LocalSocketReceiver::init(void) {

    sem_init(&sem_, 0, 1);

    struct sockaddr_un server;
    int s;

    // setup socket address structure
    memset(&server, 0, sizeof (server));
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, file_.c_str());

    // create socket
    s = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (!s) {
        perror("socket");
        exit(-1);
    }

    /* ATTENTION!!! THIS ACTUALLY REMOVES A FILE FROM YOUR HARD DRIVE!!! */
    unlink(file_.c_str()); /* Remove any previous socket with the same filename. */

    int optval = 1;
    int value = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(s, (const struct sockaddr *) & server, sizeof (server)) < 0) {
        perror("Bind");
        exit(-1);
    }
    if (listen(s, SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }

    struct thread_spawn_obj obj;
    obj.sock = s;
    obj.receiver = this;
    obj.ts = &ts_;
    sem_init(&(obj.sem), 0, 0);

    if (pthread_create(&spawner_, NULL, &thread_spawner, &obj) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }
    cout << "Thread created in init: " << spawner_ << endl;
    ts_.push(make_pair(spawner_, s));
    sem_wait(&(obj.sem));
}

void * thread_spawner(void* arg) {
    struct sockaddr_un client;
    socklen_t clientlen = sizeof (client);

    struct thread_spawn_obj * obj = (struct thread_spawn_obj *) arg;

    LocalSocketReceiver * receiver = obj->receiver;
    int socket = obj->sock;
    LocalSocketReceiverThreadsSockets* ts = obj->ts;
    int c;

    sem_post(&(obj->sem));

    while ((c = accept(socket, (struct sockaddr *) & client, &clientlen)) > 0) {
        struct thread_connection_handler_obj obj;
        obj.conn = c;
        obj.receiver = receiver;

        pthread_t thread;
        if (pthread_create(&thread, NULL, &thread_handler, &obj) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }
        cout << "Thread created in spawner: " << thread << endl;
        ts->push(make_pair(thread, c));
    }
}

void * thread_handler(void* arg) {

    LocalSocketReceiver * receiver = ((thread_connection_handler_obj *) arg)->receiver;
    int connection = ((thread_connection_handler_obj *) arg)->conn;

    char buf[MESSAGE_SIZE];
    int nread;

    while (1) {
        memset(buf, 0, MESSAGE_SIZE);
        nread = recv(connection, buf, MESSAGE_SIZE, 0);
        if (nread < 0) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else if (nread == 0) {
            // the socket is closed
            break;
        }

        string s(buf);
        receiver->recv(s);
    }
    close(connection);
}

