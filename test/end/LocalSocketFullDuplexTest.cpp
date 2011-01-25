/* 
 * File:   LocalSocketFullDuplexTest.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 3:57 PM
 */

#ifndef LOCALSOCKETFULLDUPLEXTEST_H
#define	LOCALSOCKETFULLDUPLEXTEST_H


#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../headers/LocalSocketFullDuplex.h"

using namespace std;

namespace {

    class LocalSocketFullDuplexImpl1 : public LocalSocketFullDuplex {
    public:
        LocalSocketFullDuplexImpl1(string& file) : LocalSocketFullDuplex(file) {

        }

        virtual ~LocalSocketFullDuplexImpl1() {

        }

        void receive(string& message) {
            last_received_ = message;
        }

        string& get_last_received() {
            return last_received_;
        }

    private:
        string last_received_;

    };

	TEST(LocalSocketFullDuplexTest, all) {

		string file1("LSFDF1");

		LocalSocketFullDuplexImpl1 s1(file1);

		string message = "This is a test message";

		ssize_t sent = s1.send_to(file1, message);

		if(sent < 0) {
			cout << "Error in sending: " << errno << endl;
		}

		usleep(5000);

		ASSERT_EQ(message.size(), sent);
		ASSERT_EQ(message, s1.get_last_received());


	}
}

#endif	/* LOCALSOCKETFULLDUPLEXTEST_H */

