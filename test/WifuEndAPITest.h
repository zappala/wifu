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
        last_message_ = message;
        cout << message << endl;
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
            int result = wifu_sendto(0, 0, 0, 0, 0, 0);
            cout << result << endl;
//            CHECK(result == 0);

            string file("WifuSocket");
            LocalSocketFullDuplexImpl localSocket(file);
            
            result = wifu_socket(0,0,0);
            
        }

    }
}


#endif	/* _WIFUENDAPITEST_H */

