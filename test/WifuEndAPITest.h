/* 
 * File:   WifuEndAPITest.h
 * Author: rbuck
 *
 * Created on November 20, 2010, 1:29 PM
 */

#ifndef _WIFUENDAPITEST_H
#define	_WIFUENDAPITEST_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "../headers/defines.h"

#include "UnitTest++.h"
#include "../applib/WifuEndAPI.h"

#include "../headers/LocalSocketFullDuplex.h"

using namespace std;

class LocalSocketFullDuplexImpl : public LocalSocketFullDuplex {
public:

    LocalSocketFullDuplexImpl(string& file) : LocalSocketFullDuplex(file) {
        socket_id_ = 10;
        bind_return_val_ = 100;
        listen_return_val_ = 1000;
        accept_return_val_ = 2000;
        reset();
    }

    virtual ~LocalSocketFullDuplexImpl() {

    }

    void receive(string& message) {
        //cout << "Request:\t" << message << endl;
        map<string, string> m;
        QueryStringParser::parse(message, m);

        map<string, string> response;
        response[FILE_STRING] = getFile();
        string name = m[NAME_STRING];
        response[SOCKET_STRING] = m[SOCKET_STRING];

        if (!name.compare(WIFU_SOCKET_NAME)) {
            int socket = socket_id_++;
            response[SOCKET_STRING] = Utils::itoa(socket);
        } else if (!name.compare(WIFU_BIND_NAME)) {
            int return_val = bind_return_val_++;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_LISTEN_NAME)) {
            int return_val = listen_return_val_++;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_ACCEPT_NAME)) {
            int return_val = accept_return_val_++;
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_SEND_NAME)) {
            int return_val = m[BUFFER_NAME].size();
            last_message_ = m[BUFFER_NAME];
            response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
        } else if (!name.compare(WIFU_RECV_NAME)) {

            if (recv_message_.empty()) {
                response[BUFFER_NAME] = "EOF";
                response[RETURN_VALUE_STRING] = Utils::itoa(-1);
            } else {
                int n = atoi(m[N_NAME].c_str());
                response[BUFFER_NAME] = recv_message_.substr(0, n);
                response[RETURN_VALUE_STRING] = Utils::itoa(response[BUFFER_NAME].size());

                if (n >= recv_message_.size()) {
                    recv_message_ = "";
                } else {
                    recv_message_ = recv_message_.substr(n);
                }
            }
        }

        string response_message = QueryStringParser::create(name, response);
        send_to(m[FILE_STRING], response_message);
    }

    string& get_last_message() {
        return last_message_;
    }

    string& get_recv_message() {
        return recv_message_;
    }

    void reset() {
        recv_message_ = "This is the recv() message";
    }

private:
    string last_message_;
    string recv_message_;
    int socket_id_;
    int bind_return_val_;
    int listen_return_val_;
    int accept_return_val_;
};

namespace {

    SUITE(WifuEndTest) {

        TEST(WifuSocketTest) {
            // wifu_socket()
            string file("WifuSocket");
            LocalSocketFullDuplexImpl localSocket(file);

            int socket = wifu_socket(0, 0, 0);
            int expected = 10;
            int result = socket;

            CHECK_EQUAL(expected, result);

            // wifu_bind()
            string address("127.0.0.1");
            int port = 5000;
            AddressPort ap(address, port);
            expected = 100;

            result = wifu_bind(socket, (struct sockaddr*) ap.get_network_struct_ptr(), sizeof (struct sockaddr_in));
            CHECK_EQUAL(expected, result);


            // wifu listen()
            result = wifu_listen(socket, 0);
            expected = 1000;
            CHECK_EQUAL(expected, result);

            // wifu_accept()
            socklen_t len = sizeof (struct sockaddr_in);
            result = wifu_accept(socket, (struct sockaddr*) ap.get_network_struct_ptr(), &len);
            expected = 2000;
            CHECK_EQUAL(expected, result);


            // wifu_send()
            string send_message = "This is the message to send";
            expected = send_message.size();
            result = wifu_send(socket, send_message.c_str(), send_message.size(), 0);
            CHECK_EQUAL(expected, result);
            CHECK_EQUAL(send_message, localSocket.get_last_message());

            // wifu_recv()
            string message = localSocket.get_recv_message();
            expected = localSocket.get_recv_message().size();


            for (int i = 1; i < 100; i++) {
                result = 0;
                localSocket.reset();

                char buf[i + 1];
                string result_string = "";

                while (1) {
                    memset(buf, 0, i + 1);
                    ssize_t num = wifu_recv(socket, &buf, i, 0);
                    if (num <= 0) {
                        break;
                    }

                    result += num;
                    result_string.append(buf);
                }
                CHECK_EQUAL(expected, result);
                CHECK_EQUAL(message, result_string);
            }
        }
    }
}


#endif	/* _WIFUENDAPITEST_H */

