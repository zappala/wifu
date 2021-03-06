#include "RawSocketListener.h"

void* listener(void* arg) {
    struct listen_data* data = (struct listen_data*) arg;
    NetworkCallback* callback = data->callback;
    int epfd = data->epfd;
    Semaphore* sem = data->sem;
    map<int, PacketFactory*, std::less<int>, gc_allocator<std::pair<int, PacketFactory*> > >* factories = data->factories;

    sem->post();

    int ret;
    WiFuPacket* packet;
    PacketFactory* factory;

    int nfds, fd;
    struct epoll_event events[MAX_EVENTS];

    while (1) {
        nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

        if (nfds < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("RawSocketListener: epoll_wait()");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {

            fd = events[i].data.fd;

            factory = factories->operator [](fd);
            packet = factory->create();

            ret = recv(fd, packet->get_payload(), MTU, 0);

            assert(ret == packet->get_ip_tot_length());

            callback->imodule_network_receive(packet);
        }
    }
}

RawSocketListener::RawSocketListener() : started_(false) {
    sem_ = new Semaphore();
    sem_->init(0);
    epfd_ = epoll_create(1);
}

RawSocketListener::~RawSocketListener() {
    while (!fds_.empty()) {
        int fd = fds_.front();
        fds_.pop_front();
        close(fd);
    }
    close(epfd_);
    pthread_cancel(thread_);
}

void RawSocketListener::register_protocol(int protocol, PacketFactory* pf) {
    if (started_) {
        cout << "Cannot register anymore protocols" << endl;
        // TODO: throw an exception
        assert(false);
        return;
    }

    int fd = socket(AF_INET, SOCK_RAW, protocol);
    if (fd < 0) {
        perror("RawSocketListener: Cannot create socket");
        exit(EXIT_FAILURE);
    }


    event_.events = EPOLLIN;
    event_.data.fd = fd;
    epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event_);

    factories_[fd] = pf;
    fds_.push_back(fd);
}

void RawSocketListener::start(NetworkCallback* callback) {
    started_ = true;

    struct listen_data data;
    data.callback = callback;
    data.epfd = epfd_;
    data.sem = sem_;
    data.factories = &factories_;

    if (pthread_create(&thread_, NULL, &listener, &data) != 0) {
        perror("RawSocketListener: cannot create thread");
        exit(EXIT_FAILURE);
    }

    sem_->wait();
}
