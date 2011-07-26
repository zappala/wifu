#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

void* kernel_active_to_passive_big_chunks_thread(void* args) {


    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    Semaphore* flag = v->flag_;
    Semaphore* done = v->done_;

    gcstring expected = v->expected_string;

    // Create server
    int server = socket(AF_INET, SOCK_STREAM, 6);
    int result = bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    EXPECT_EQ(0, result);
    result = listen(server, 5);
    EXPECT_EQ(0, result);

    sem->post();

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    if ((connection = accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    flag->post();



    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);
    //    cout << "Connected to: " << ap.to_s() << endl;

    int size = 50000;
    char buffer[size];
    gcstring all_received = "";

    list<u_int64_t, gc_allocator<u_int64_t> > durations;

    Timer recv_timer;

    while (true) {

        memset(buffer, 0, size);
        u_int64_t start = Utils::get_current_time_microseconds_64();
        int return_value = recv(connection, &buffer, 10000, 0);
        durations.push_back(Utils::get_current_time_microseconds_64() - start);
        recv_timer.start();

        if (return_value == 0) {
//                        cout << "Close Thread BREAK" << endl;
            break;
        }

//        gcstring actual(buffer);
//        all_received.append(actual);
    }

    recv_timer.stop();

    // get rid of the first sample, it might have been delayed.
    durations.pop_front();
    // get rid of the last sample as it is a return of 0
    durations.pop_back();
    
    u_int64_t total = 0;
    u_int64_t durations_size = durations.size();
    while (!durations.empty()) {
        total += durations.front();
        durations.pop_front();
    }

    cout << "Average on wifu to call and return from recv(): " << (total / durations_size) << endl;

    cout << "Duration (us) to recv: " << expected.size() << " bytes on localhost: " << recv_timer.get_duration_microseconds() << endl;

    close(connection);
    close(server);
//    EXPECT_EQ(expected, all_received);
    done->post();
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void kernel_active_to_passive_big_chunks(int protocol, gcstring message) {
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

    if (pthread_create(&(t), NULL, &kernel_active_to_passive_big_chunks_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    // ensure all variables are copied
    v.sem_->wait();

    // ensure we are in accept
    usleep(50000);

    // Create client
    timer.start();
    client = socket(AF_INET, SOCK_STREAM, 6);
    result = connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    v.flag_->wait();

    cout << "Duration (us) to create a socket and connect on localhost via kernel: " << timer.get_duration_microseconds() << endl;

    int index = 0;
    int chunk = 10000;
    int num_sent = 0;


    Timer send_timer;
    send_timer.start();

    while(index < message.length()) {

        if(index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

        int sent = send(client, data, chunk, 0);
        num_sent += sent;
        index += sent;
    }
    send_timer.stop();
    cout << "Duration (us) to send: " << message.size() << " bytes on localhost: " << send_timer.get_duration_microseconds() << endl;
//    cout << "Done sending" << endl;

    EXPECT_EQ(message.length(), num_sent);

    close(client);
    v.done_->wait();
}

TEST_F(BackEndTest, kernelSendReceiveTestActiveBigChunks100000) {
    kernel_active_to_passive_big_chunks(6, random_string(100000));
}

TEST_F(BackEndTest, kernelSendReceiveTestActiveBigChunks1000000) {
    kernel_active_to_passive_big_chunks(6, random_string(1000000));
}

TEST_F(BackEndTest, kernelSendReceiveTestActiveBigChunks10000000) {
    kernel_active_to_passive_big_chunks(6, random_string(10000000));
}
