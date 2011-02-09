#include "RawSocketListener.h"

void* listener(void* arg) {
    struct listen_data* data = (struct listen_data*) arg;
    NetworkCallback* callback = data->callback;
    int epfd = data->epfd;
    Semaphore* sem = data->sem;
    map<int, PacketFactory*>* factories = data->factories;

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

            ret = recv(fd, packet->get_payload(), PAYLOAD_SIZE, 0);
            if (ret <= 0) {
                assert(false);
            }
            packet->set_ip_datagram_length(ret);

            callback->network_receive(packet);
        }
    }
}

RawSocketListener::RawSocketListener() : started_(false) {
	sem_ = new Semaphore();
	sem_->init(0);
	epfd_ = epoll_create(1);
}

void RawSocketListener::register_protocol(int protocol, PacketFactory* pf) {
	if (started_) {
		cout << "Cannot register anymore protocols" << endl;
		// TODO: throw an exception
		return;
	}

	int fd = socket(AF_INET, SOCK_RAW, protocol);
	if (fd < 0) {
		perror("RawSocketListener: Cannot create socket");
		exit(EXIT_FAILURE);
	}

	static struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event);

	factories_[fd] = pf;
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
