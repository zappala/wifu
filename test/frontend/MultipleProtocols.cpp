#include "../headers/MultipleProtocols.h"

struct item {
    gcstring message;
    pthread_barrier_t* barrier;
};


void* tcp_tahoe_send_thread(void* args) {
cout << "tahoe send thread" << endl;
    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    pthread_barrier_t* barrier = v->barrier;

    gcstring message = v->expected_string;

    // Create server
    int server = wifu_socket(AF_INET, SOCK_STREAM, v->protocol_);
    int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    EXPECT_EQ(0, result);
    result = wifu_listen(server, 5);
    EXPECT_EQ(0, result);
    sem->post();

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    cout << "TCP server socket:     " << server << endl;
    cout << "TCP connection socket: " << connection << endl;


    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);

    int index = 0;
    int chunk = 10000;
    int num_sent = 0;

    int value = pthread_barrier_wait(barrier);
    if (value != 0 && value != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("Could not wait on barrier before connecting");
        exit(EXIT_FAILURE);
    }

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }

        const char* data = message.data() + index;

        int sent = wifu_send(connection, data, chunk, 0);
        num_sent += sent;
        index += sent;
    }

    EXPECT_EQ(message.length(), num_sent);

//    cout << "Done Sending data" << endl;

    wifu_close(connection);
    wifu_close(server);
}


void* tcp_tahoe_receive_thread(void* arg) {
cout << "tahoe receive thread" << endl;
    struct item* itm = (struct item*) arg;
    pthread_barrier_t* barrier = itm->barrier;
    
    AddressPort to_connect("127.0.0.1", 5002);

    //cout << "Beginning test." << endl;

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.sem_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);
    v.protocol_ = TCP_TAHOE;
    v.barrier = barrier;

    //Specify the number of bytes to send here.
    v.expected_string = itm->message;


    if (pthread_create(&(t), NULL, &tcp_tahoe_send_thread, &(v)) != 0) {
        perror("Error creating new thread in IntegrationTest.h");
        exit(EXIT_FAILURE);
    }

    v.sem_->wait();

    //cout << "Creating receiver." << endl;

    // Make sure that the thread is in the accept state
    usleep(50000);

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    cout << "TCP client socket: " << client << endl;
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    EXPECT_EQ(0, result);

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 50000;
    char buffer[size];

    gcstring expected = v.expected_string;
    gcstring all_received = "";

    while (true) {
        memset(buffer, 0, size);
        int return_value = wifu_recv(client, &buffer, 10000, 0);

        if (return_value == 0) {
            break;
        }

        gcstring actual(buffer);
        all_received.append(actual);

    }

    EXPECT_EQ(expected, all_received);
    wifu_close(client);

    pthread_join(t, NULL);
}

struct udp_struct {
    gcstring message;
    AddressPort* ap;
    int packets;
    Semaphore* ready;
    pthread_barrier_t* barrier;

};

void* udp_recv_thread(void* args) {
cout << "UDP receive thread" << endl;
    struct udp_struct* info = (struct udp_struct*) args;
    gcstring message = info->message;
    AddressPort* ap = new AddressPort(info->ap->get_address(), info->ap->get_port());
    int packets = info->packets;
    Semaphore* ready = info->ready;
    pthread_barrier_t* barrier = info->barrier;


    int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    cout << "UDP receive socket number: " << socket << endl;
    if (socket < 0) {
        perror("Error creating udp socket in thread");
        exit(EXIT_FAILURE);
    }


    socklen_t length = sizeof (struct sockaddr_in);

    if (wifu_bind(socket, (struct sockaddr*) ap->get_network_struct_ptr(), length) < 0) {
        perror("Error binding udp socket");
        exit(EXIT_FAILURE);
    }


    gcstring data = "";
    int size = 1500;
    char buffer[size];
    
    ready->post();

    for (int i = 0; i < packets; i++) {
        ssize_t n = wifu_recvfrom(socket, buffer, size, 0, (struct sockaddr*) ap->get_network_struct_ptr(), & length);
        data.append(buffer, n);
        if(i > (2/3) * packets) {
            break;
        }
    }

    wifu_close(socket);
    //EXPECT_EQ(message, data);

}

void* udp_send_thread(void* arg) {
    cout << "UDP send thread" << endl;

    struct item* itm = (struct item*) arg;
    pthread_barrier_t* barrier = itm->barrier;

    Semaphore* ready = new Semaphore();
    ready->init(0);
    int packets = 1000;
    int length = 1000 * packets;
    gcstring message = RandomStringGenerator::get_data(length);
    AddressPort* receiver = new AddressPort("127.0.0.1", 5000);

    struct udp_struct info;
    info.ap = receiver;
    info.message = message;
    info.packets = packets;
    info.ready = ready;
    info.barrier = barrier;

    pthread_t thread;
    if (pthread_create(&thread, NULL, &udp_recv_thread, &info) != 0) {
        perror("Error creating thread in UDPTest");
        exit(EXIT_FAILURE);
    }

    int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    cout << "UDP send socket number: " << socket << endl;
    if (socket < 0) {
        perror("Error creating udp socket");
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof (struct sockaddr_in);

    int index = 0;
    int increment = 1000;
    int total_sent = 0;

    ready->wait();

    int value = pthread_barrier_wait(barrier);
    if (value != 0 && value != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("Could not wait on barrier before connecting");
        exit(EXIT_FAILURE);
    }
    
    while (index < message.size()) {
        
        int n = wifu_sendto(socket, message.data() + index, increment, 0, (struct sockaddr*) receiver->get_network_struct_ptr(), len);
        total_sent += n;
        index += increment;

    }

    wifu_close(socket);

    EXPECT_EQ(message.size(), total_sent);

    pthread_join(thread, NULL);
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void mutliple_protocols(gcstring message) {
    pthread_t tcp, udp;

    pthread_barrier_t barrier;
    if (pthread_barrier_init(&barrier, NULL, 2)) {
        perror("Error creating barrier");
        exit(EXIT_FAILURE);
    }

    struct item itm;
    itm.barrier = &barrier;
    itm.message = message;


    if (pthread_create(&tcp, NULL, &tcp_tahoe_receive_thread, &itm) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    
    if (pthread_create(&udp, NULL, &udp_send_thread, &itm) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    pthread_join(udp, NULL);
    pthread_join(tcp, NULL);
}


TEST_F(BackEndTest, multipleProtocols1000000) {
    mutliple_protocols(random_string(1000000));
    sleep(5);
}