#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

void* tahoe_passive_to_active_thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

    string message = v->expected_string;

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

    AddressPort ap(&addr);
    string address("127.0.0.1");
    string res = ap.get_address();
    EXPECT_EQ(address, res);
//    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];

    memcpy(buffer, message.c_str(), message.length());

    int count = 1;
    int num_sent = 0;
    // TODO: this only sends one character at a time
    for (int i = 0; i < message.length(); i++) {

        num_sent += wifu_send(connection, &(buffer[i]), count, 0);
    }

    EXPECT_EQ(message.length(), num_sent);
//    cout << "SendReceivePassiveToActive, sent message: " << message << endl;
    wifu_close(connection);
    wifu_close(server);
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void tahoe_passive_to_active_send_receive_test(int protocol, string message) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.sem_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);

    v.protocol_ = protocol;
    //Specify the number of bytes to send here.
    v.expected_string = message;


    if (pthread_create(&(t), NULL, &tahoe_passive_to_active_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    v.sem_->wait();

    // Make sure that the thread is in the accept state
    usleep(50000);

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, protocol);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    EXPECT_EQ(0, result);

//    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 1500;
    char buffer[size];

    string expected = v.expected_string;
    string all_received = "";

    while(true) {
        memset(buffer, 0, size);
        int return_value = wifu_recv(client, &buffer, 1, 0);

        if(return_value == 0) {
//            cout << "BREAK" << endl;
            break;
        }

        string actual(buffer);
        all_received.append(actual);
    }

    EXPECT_EQ(expected, all_received);
    wifu_close(client);
}


TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(1));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(5));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(10));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(20));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(100));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(2000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(5000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom1Percent, tahoeSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom5Percent, tahoeSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom10Percent, tahoeSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom20Percent, tahoeSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom30Percent, tahoeSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_TAHOE, random_string(50000));
}
