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

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"

#include "../headers/LocalSocketFullDuplex.h"
#include "../headers/Utils.h"
#include "../headers/QueryStringParser.h"
#include "../headers/AddressPort.h"

using namespace std;

namespace {

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
				response[ADDRESS_STRING] = "127.0.0.1";
				response[PORT_STRING] = "9000";
			} else if (!name.compare(WIFU_SENDTO_NAME)) {
				int return_val = m[BUFFER_STRING].size();
				last_message_ = m[BUFFER_STRING];
				response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
			} else if (!name.compare(WIFU_RECVFROM_NAME)) {

				if (recv_message_.empty()) {
					response[BUFFER_STRING] = "EOF";
					response[RETURN_VALUE_STRING] = Utils::itoa(-1);
				} else {
					int n = atoi(m[N_STRING].c_str());
					response[BUFFER_STRING] = recv_message_.substr(0, n);
					response[RETURN_VALUE_STRING] = Utils::itoa(response[BUFFER_STRING].size());

					if (n >= recv_message_.size()) {
						recv_message_ = "";
					} else {
						recv_message_ = recv_message_.substr(n);
					}
				}
			} else if (!name.compare(WIFU_CONNECT_NAME)) {
				int return_val = 0;
				response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
			} else if (!name.compare(WIFU_GETSOCKOPT_NAME)) {
				int return_val = SO_BINDTODEVICE;
				string value = "Value";
				response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
				response[BUFFER_STRING] = value;
			} else if (!name.compare(WIFU_SETSOCKOPT_NAME)) {
				int return_val = 0;
				response[RETURN_VALUE_STRING] = Utils::itoa(return_val);
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

	TEST(WifuSocketTest, allTests) {
		// wifu_socket()
		string file("WS");
		LocalSocketFullDuplexImpl localSocket(file);

		int socket = wifu_socket(1000, 1000, SIMPLE_TCP);
		int expected = 10;
		int result = socket;

		ASSERT_EQ(expected, result);

		// wifu_bind()
		string address("127.0.0.1");
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
		result = wifu_accept(socket, (struct sockaddr*) &s, &len);
		expected = 2000;
		ASSERT_EQ(expected, result);
		AddressPort exp("127.0.0.1", 9000);
		AddressPort act(&s);
		ASSERT_EQ(exp.to_s(), act.to_s());


		// Do everything on new socket
		socket = expected;


		// wifu_send()
		string send_message = "This is the message to send";
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
		string message = localSocket.get_recv_message();
		expected = localSocket.get_recv_message().size();

		for (int i = 1; i <= 50; i++) {
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
			ASSERT_EQ(expected, result);
			ASSERT_EQ(message, result_string);
		}

		// wifu_recvfrom()
		for (int i = 1; i <= 50; i++) {
			result = 0;
			localSocket.reset();

			char buf[i + 1];
			string result_string = "";

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

		// wifu_connect()
		expected = 0;
		result = wifu_connect(socket, (struct sockaddr*) ap.get_network_struct_ptr(), len);
		ASSERT_EQ(expected, result);

		//wifu_getsockopt()
		expected = SO_BINDTODEVICE;
		const char* optvalue = "Value";
		socklen_t val_len = 5;

		socklen_t length = 1000;
		char buf[length];
		memset(buf, 0, length);
		result = wifu_getsockopt(socket, 0, SO_BINDTODEVICE, &buf, &length);
		ASSERT_EQ(expected, result);
		ASSERT_STREQ(optvalue, buf);
		ASSERT_EQ(val_len, length);

		//wifu_setsockopt()
		expected = 0;
		result = wifu_setsockopt(socket, 0, SO_BINDTODEVICE, optvalue, val_len);
		ASSERT_EQ(expected, result);
	}
}


#endif	/* _WIFUENDAPITEST_H */

