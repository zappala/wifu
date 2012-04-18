/* 
 * File:   WiFuTransportAPITest.h
 * Author: rbuck
 *
 * Created on November 20, 2010, 1:29 PM
 */

#ifndef _WIFUTRANSPORTAPITEST_H
#define	_WIFUTRANSPORTAPITEST_H

#include <iostream>
#include <unistd.h>
#include "../headers/defines.h"

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"

#include "../headers/LocalSocketFullDuplex.h"
#include "../headers/Utils.h"
#include "../headers/QueryStringParser.h"
#include "../headers/AddressPort.h"
#include "events/framework_events/AddressResponseEvent.h"

#include "../headers/events/framework_events/ResponseEvent.h"
#include "../headers/events/framework_events/AcceptResponseEvent.h"
#include "../headers/events/framework_events/RecvFromResponseEvent.h"

#include "../headers/MessageStructDefinitions.h"


using namespace std;

namespace {

    class LocalSocketFullDuplexImpl : public LocalSocketFullDuplex {
    public:

        LocalSocketFullDuplexImpl(gcstring& file) : LocalSocketFullDuplex(file) {
            socket_id_ = 10;
            bind_return_val_ = 100;
            listen_return_val_ = 1000;
            accept_return_val_ = 2000;
            reset();

        }

        virtual ~LocalSocketFullDuplexImpl() {

        }

        void receive(unsigned char* message, int length) {
            struct GenericMessage* gm = (struct GenericMessage*) message;

            ResponseEvent* response = new ResponseEvent();

            struct SocketMessage* sm = (struct SocketMessage*) message;
            response->set_message_type(sm->message_type);
            response->set_fd(sm->fd);
            response->set_errno(0);
            response->set_default_length();
            response->set_destination(&(sm->source));

            switch (gm->message_type) {
                case WIFU_RECVFROM:
                case WIFU_RECV:
                {
                    struct RecvFromMessage* rfm = (struct RecvFromMessage*) sm;
                    RecvFromResponseEvent* rfre = (RecvFromResponseEvent*) response;

                    if (recv_message_.empty()) {
                        rfre->set_return_value(-1);
                    } else {

                        int n = rfm->buffer_length;

                        gcstring s = recv_message_.substr(0, n);
                        rfre->set_return_buffer((unsigned char*) s.c_str(), s.size());
                        if (n >= recv_message_.size()) {
                            recv_message_ = "";
                        } else {
                            recv_message_ = recv_message_.substr(n);
                        }
                    }

                    break;
                }
                case WIFU_SENDTO:
                case WIFU_SEND:
                {
                    struct SendToMessage* stm = (struct SendToMessage*) sm;
                    int length = stm->buffer_length;
                    response->set_return_value(length);
                    last_message_ = gcstring((char*) (stm + 1), length);
                    break;
                }
                case WIFU_SOCKET:
                    response->set_return_value(socket_id_++);
                    break;

                case WIFU_BIND:
                    response->set_return_value(bind_return_val_++);
                    break;

                case WIFU_LISTEN:
                    response->set_return_value(listen_return_val_++);
                    break;

                case WIFU_ACCEPT:
                {
                    AcceptResponseEvent* are = (AcceptResponseEvent*) response;
                    AddressPort ap("127.0.0.1", 9000);
                    are->set_addr(ap.get_network_struct_ptr(), sizeof (struct sockaddr_in));
                    are->set_return_value(accept_return_val_++);
                    break;
                }

                case WIFU_CONNECT:
                    break;

                case WIFU_GETSOCKOPT:
                    break;

                case WIFU_SETSOCKOPT:
                    break;

                case WIFU_CLOSE:
                    break;

                default:
                    throw WiFuException("Unknown message type");
            }
            send_to(response->get_destination(), response->get_buffer(), response->get_length());
        }

        gcstring& get_last_message() {
            return last_message_;
        }

        gcstring& get_recv_message() {
            return recv_message_;
        }

        void reset() {
            recv_message_ = "This is the recv() message";
        }

    private:
        gcstring last_message_;
        gcstring recv_message_;
        int socket_id_;
        int bind_return_val_;
        int listen_return_val_;
        int accept_return_val_;
    };

    TEST(WifuSocketTest, allTests) {
        // wifu_socket()
        gcstring file("/tmp/WS");
        LocalSocketFullDuplexImpl localSocket(file);

        int socket = wifu_socket(1000, 1000, TCP_TAHOE);
        int expected = 10;
        int result = socket;

        ASSERT_EQ(expected, result);

        // wifu_bind()
        gcstring address("127.0.0.1");
        int port = 5000;
        AddressPort ap(address, port);
        expected = 100;

        result = wifu_bind(socket, (struct sockaddr*) ap.get_network_struct_ptr(), sizeof (struct sockaddr_in));
        ASSERT_EQ(expected, result);


        // wifu listen()
        result = wifu_listen(socket, 0);
        expected = 1000;
        ASSERT_EQ(expected, result);

        // wifu_accept()
        socklen_t len = sizeof (struct sockaddr_in);
        struct sockaddr_in s;
        result = wifu_accept(socket, (struct sockaddr*) & s, &len);
        expected = 2000;
        ASSERT_EQ(expected, result);
        AddressPort exp("127.0.0.1", 9000);
        AddressPort act(&s);
        ASSERT_EQ(exp.to_s(), act.to_s());


        // Do everything on new socket
        socket = expected;


        // wifu_send()
        gcstring send_message = "This is the message to send";
        expected = send_message.size();
        result = wifu_send(socket, send_message.c_str(), send_message.size(), 0);
        ASSERT_EQ(expected, result);
        ASSERT_EQ(send_message, localSocket.get_last_message());

        // wifu_sendto()
        len = sizeof (struct sockaddr_in);
        result = wifu_sendto(socket, send_message.c_str(), send_message.size(), 0, (struct sockaddr*) ap.get_network_struct_ptr(), len);
        expected = send_message.size();
        ASSERT_EQ(expected, result);
        ASSERT_EQ(send_message, localSocket.get_last_message());

        // wifu_recv()
        gcstring message = localSocket.get_recv_message();
        expected = localSocket.get_recv_message().size();

        for (int i = 1; i <= 50; i++) {
            result = 0;
            localSocket.reset();

            char buf[i + 1];
            gcstring result_string = "";

            while (1) {
                memset(buf, 0, i + 1);
                ssize_t num = wifu_recv(socket, &buf, i, 0);
                if (num <= 0) {
                    break;
                }

                result += num;
                result_string.append(buf);
            }
            ASSERT_EQ(expected, result);
            ASSERT_EQ(message, result_string);
        }

        // wifu_recvfrom()
        // TODO: check the value of the address and length
        for (int i = 1; i <= 50; i++) {
            result = 0;
            localSocket.reset();

            char buf[i + 1];
            gcstring result_string = "";

            while (1) {
                memset(buf, 0, i + 1);
                ssize_t num = wifu_recvfrom(socket, &buf, i, 0, (struct sockaddr*) ap.get_network_struct_ptr(), &len);
                if (num <= 0) {
                    break;
                }

                result += num;
                result_string.append(buf);
            }
            ASSERT_EQ(expected, result);
            ASSERT_EQ(message, result_string);
        }

        // TODO: Add support for the rest of the calls

//        // wifu_connect()
//        expected = 0;
//        result = wifu_connect(socket, (struct sockaddr*) ap.get_network_struct_ptr(), len);
//        ASSERT_EQ(expected, result);

    }
}


#endif	/* _WIFUTRANSPORTAPITEST_H */

