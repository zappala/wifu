/* 
 * File:   IntegrationTest.h
 * Author: rbuck
 *
 * Created on December 2, 2010, 9:03 AM
 */

#ifndef _INTEGRATIONTEST_H
#define	_INTEGRATIONTEST_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"
#include "../headers/AddressPort.h"
#include "../headers/GarbageCollector.h"
#include "../headers/Semaphore.h"
#include "Timer.h"

using namespace std;

namespace {

	struct var {
		Semaphore* sem_;
		AddressPort* to_bind_;
	};

	class BackEndTest : public ::testing::Test {
	protected:
		virtual void SetUp() {
			start_backend();
		}

		virtual void TearDown() {
			kill_backend();
		}

	    virtual void start_backend() {
//	    	bool notFound;
//	    	string ls = getOutputFromCommand("ls wifu-end");
//	    	int returned = ls.find("No such file or directory");
//	    	notFound = returned == string::npos ? true : false;
//
	    	string commandToExecute;
//	    	if (notFound)
//	    		commandToExecute = "./bin/wifu-end";
//	    	else
//	    		commandToExecute = "./wifu-end";
//	    	commandToExecute = "wifu-end";
	    	commandToExecute = "./bin/wifu-end";

	        int value = system(commandToExecute.c_str());
	        if (value < 0)
	            FAIL() << "Error starting wifu-end";
	        // We have to sleep so we can ensure the back end is up and running
	        sleep(1);
	    }

	    virtual void kill_backend() {
	        int value = system("killall wifu-end");
	        if (value < 0)
	            FAIL() << "Error killing wifu-end";
	    }

	private:
	    string getOutputFromCommand(char* cmd) {
	    	//_popen on Windows
	        FILE* pipe = popen(cmd, "r");
	        if (!pipe)
	        	ADD_FAILURE() << "Error opening pipe to find wifu-end";
	        char buffer[128];
	        std::string result = "";
	        while(!feof(pipe))
	        	if(fgets(buffer, 128, pipe) != NULL)
	            	result += buffer;
	        //_pclose on Windows
	        pclose(pipe);
	        return result;
	    }
	};

    TEST_F(BackEndTest, socketTest) {
    	// socket
		for (int i = 0; i < 100; i++) {
			// Check valid
			int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
			ASSERT_TRUE(socket >= 0);

			// Check invalid (i != SIMPLE_TCP)
			socket = wifu_socket(AF_INET, SOCK_STREAM, i);
			ASSERT_TRUE(socket == -1);
		}
    }

    TEST_F(BackEndTest, bindTest) {
        struct sockaddr_in to_bind;
        socklen_t length = sizeof (struct sockaddr_in);
        memset(&to_bind, 0, length);

        to_bind.sin_family = AF_INET;
        to_bind.sin_port = htons(5000);
        //TODO: do we want to ever bind on any other address?
        to_bind.sin_addr.s_addr = INADDR_ANY;

        // Bad fd
        //TODO: need to close all the sockets opened up above as we may collide
        // (badfd may actually have been created above)
        int badfd = 4000;
        int result = wifu_bind(badfd, (const struct sockaddr *) & to_bind, length);
        ASSERT_EQ(EBADF, errno);
        ASSERT_TRUE(result < 0);

        // Should be successful
        int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        ASSERT_EQ(0, result);


        // Should fail (already bound)
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        ASSERT_TRUE(result < 0);
        ASSERT_EQ(EINVAL, errno);
    }

    TEST_F(BackEndTest, listenTest) {
        // Bad fd
        // TODO: need to close all the sockets opened up above as we may collide
        // (badfd may actually have been created above)
        int badfd = 4000;
        int queue_size = 5;
        int result = wifu_listen(badfd, queue_size);
        ASSERT_EQ(EBADF, errno);
        ASSERT_TRUE(result < 0);

        struct sockaddr_in to_bind;
        socklen_t length = sizeof (struct sockaddr_in);
        memset(&to_bind, 0, length);
        to_bind.sin_family = AF_INET;
        to_bind.sin_port = htons(5001);
        to_bind.sin_addr.s_addr = INADDR_ANY;

        // Should be successful
        int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
        ASSERT_EQ(0, result);
        result = wifu_listen(socket, 5);
        ASSERT_EQ(0, result);

        // Should fail
        result = wifu_listen(socket, 5);
        ASSERT_TRUE(result < 0);
        ASSERT_EQ(EADDRINUSE, errno);
    }

    void* thread(void* args) {

        struct var* v = (struct var*) args;
        AddressPort* to_bind = v->to_bind_;
        Semaphore* sem = v->sem_;

        // Create server
        int server = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
        EXPECT_EQ(0, result);
        result = wifu_listen(server, 5);
        EXPECT_EQ(0, result);
        sem->post();

        struct sockaddr_in addr;
        socklen_t length = sizeof (addr);
        int connection;
		if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0)
			ADD_FAILURE() << "Problem in Accept";

        // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented
    }

    TEST_F(BackEndTest, connectTest) {
        AddressPort to_connect("127.0.0.1", 5002);

        struct var v;
        v.sem_ = new Semaphore();
        v.sem_->init(0);
        v.to_bind_ = new AddressPort("127.0.0.1", 5002);

        pthread_t t;
        if (pthread_create(&t, NULL, &thread, &v) != 0)
            FAIL() << "Error creating new thread in IntegrationTest.h";

        v.sem_->wait();

        // Make sure that the thread is in the accept state
        usleep(50000);

        // Create client
        Timer timer;
        timer.start();
        int client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        int result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
        timer.stop();
        ASSERT_EQ(0, result);

        cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;
    }

	void* compare_thread(void* args) {

		struct var* v = (struct var*) args;
		AddressPort* to_bind = v->to_bind_;
		Semaphore* sem = v->sem_;

		// Create server
		int server = socket(AF_INET, SOCK_STREAM, 0);
		int result = bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
		EXPECT_EQ(0, result);
		result = listen(server, 5);
		EXPECT_EQ(0, result);
		sem->post();

		struct sockaddr_in addr;
		socklen_t length = sizeof (addr);
		int connection;
		if ((connection = accept(server, (struct sockaddr *) & addr, &length)) < 0)
			ADD_FAILURE() << "Problem in Accept";

		close(connection);
		close(server);

		// TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented
	}

    TEST_F(BackEndTest, compareTest) {
        AddressPort to_connect("127.0.0.1", 5002);

        struct var v;
        v.sem_ = new Semaphore();
        v.sem_->init(0);
        v.to_bind_ = new AddressPort("127.0.0.1", 5002);

        pthread_t t;
        if (pthread_create(&t, NULL, &compare_thread, &v) != 0)
            FAIL() << "Error creating new thread in IntegrationTest.h";

        v.sem_->wait();

        // Make sure that the thread is in the accept state
        usleep(50000);

        // Create client
        Timer timer;
        timer.start();
        int client = socket(AF_INET, SOCK_STREAM, 0);
        int result = connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
        timer.stop();
        ASSERT_EQ(0, result);

        cout << "Duration (us) to create a socket and connect on localhost via kernel: " << timer.get_duration_microseconds() << endl;
        close(client);
    }

}


#endif	/* _INTEGRATIONTEST_H */

