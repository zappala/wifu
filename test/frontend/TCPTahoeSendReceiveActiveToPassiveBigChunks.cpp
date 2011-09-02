#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

void* tahoe_active_to_passive_big_chunks_thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    Semaphore* flag = v->flag_;
    Semaphore* done = v->done_;
    gcstring expected;
    expected.reserve(v->expected_string.size());
    expected = v->expected_string;

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
        assert(false);
    }

    flag->post();

    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);
    //    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];
    gcstring all_received = "";

    list<u_int64_t, gc_allocator<u_int64_t> > durations;

    Timer recv_timer;
    while (true) {

        memset(buffer, 0, size);
        u_int64_t start = Utils::get_current_time_microseconds_64();
        int return_value = wifu_recv(connection, buffer, 10000, 0);
        durations.push_back(Utils::get_current_time_microseconds_64() - start);
        recv_timer.start();

        if (return_value == 0) {
            break;
        }
        all_received.append(buffer);
    }

    recv_timer.stop();

    cout << "Duration (us) to recv: " << expected.size() << " bytes on localhost: " << recv_timer.get_duration_microseconds() << endl;

    wifu_close(connection);
    wifu_close(server);
    EXPECT_EQ(expected, all_received);
    done->post();
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void tahoe_active_to_passive_big_chunks(int protocol, gcstring message) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.flag_ = new Semaphore();
    v.done_ = new Semaphore();
    v.sem_->init(0);
    v.flag_->init(0);
    v.done_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);
    v.protocol_ = protocol;

    //Specify the number of bytes to send here.
    v.expected_string = message;

    if (pthread_create(&(t), NULL, &tahoe_active_to_passive_big_chunks_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    // ensure all variables are copied
    v.sem_->wait();

    // ensure we are in accept
    usleep(50000);

    // Create client
    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, protocol);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    v.flag_->wait();

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int index = 0;
    int chunk = 10000;
    int num_sent = 0;


    Timer send_timer;
    send_timer.start();

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

        int sent = wifu_send(client, data, chunk, 0);
        num_sent += sent;
        index += sent;
    }
    send_timer.stop();
    cout << "Duration (us) to send: " << message.size() << " bytes on localhost: " << send_timer.get_duration_microseconds() << endl;

    EXPECT_EQ(message.length(), num_sent);
    wifu_close(client);
    assert(v.done_);
    v.done_->wait();
}

TEST_F(BackEndTest, tahoeSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(100000));
}

TEST_F(BackEndTest, tahoeSendReceiveTestActiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(1000000));
}

TEST_F(BackEndTest, tahoeSendReceiveTestActiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(10000000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(10000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks20000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(20000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks30000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(30000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(65535));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(87380));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(100000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(1000000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(10000000));
}

TEST_F(BackEndMockTestDropRandom10Percent, tahoeSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(10000));
}

TEST_F(BackEndMockTestDropRandom10Percent, tahoeSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(100000));
}

TEST_F(BackEndMockTestDropRandom20Percent, tahoeSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_TAHOE, random_string(100000));
}