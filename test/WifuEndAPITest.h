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

    }

    virtual ~LocalSocketFullDuplexImpl() {

    }

    void receive(string& message) {
        map<string, string> m;
        QueryStringParser::parse(message, m);

        map<string, string> response;
        response[FILE_STRING] = getFile();
        string name = m[NAME_STRING];
        response[SOCKET_STRING] = m[SOCKET_STRING];

        if (!name.compare("wifu_socket")) {
            int socket = 9;
            response[SOCKET_STRING] = Utils::itoa(socket);
        } else if (!name.compare("wifu_bind")) {
            int r = 100;
            response[RETURN_VALUE_STRING] = Utils::itoa(r);
        }

        string response_message = QueryStringParser::create(name, response);
        send_to(m[FILE_STRING], response_message);
    }

    string& get_last_message() {
        return last_message_;
    }

private:
    string last_message_;
};

namespace {

    SUITE(WifuEndTest) {

        TEST(WifuSocketTest) {
            // wifu_socket()
            string file("WifuSocket");
            LocalSocketFullDuplexImpl localSocket(file);

            int result = wifu_socket(0, 0, 0);
            int expected = 9;

            CHECK_EQUAL(expected, result);

            // wifu_bind()
            string address("127.0.0.1");
            int port = 5000;
            AddressPort ap(address, port);
            expected = 100;

            result = wifu_bind(9, (struct sockaddr*) ap.get_network_struct_ptr(), sizeof (struct sockaddr_in));
            CHECK_EQUAL(expected, result);
        }

      

    }
}


#endif	/* _WIFUENDAPITEST_H */

