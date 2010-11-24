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
        string name;

        if (!m[NAME_STRING].compare("wifu_socket")) {
            int socket = 9;
            response["socket"] = Utils::itoa(socket);
            name = "wifu_socket";
        }

        message = QueryStringParser::create(name, response);
        send_to(m[FILE_STRING], message);
    }

    string& get_last_message() {
        return last_message_;
    }

private:
    string last_message_;
};

namespace {

    SUITE(WifuEndTest) {

        TEST(WifuEndTest) {
            string file("WifuSocket");
            LocalSocketFullDuplexImpl localSocket(file);

            int result = wifu_socket(0, 0, 0);
            int expected = 9;

            CHECK_EQUAL(expected, result);
        }

    }
}


#endif	/* _WIFUENDAPITEST_H */

