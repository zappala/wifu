/* 
 * File:   TemporaryStateTest.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 4:15 PM
 */

#ifndef TEMPORARYSTATETEST_H
#define	TEMPORARYSTATETEST_H


#include "UnitTest++.h"
#include "../headers/contexts/TCP.h"

using namespace std;

namespace {

    SUITE(TemporaryStateTest) {

        TEST(TemporaryStateTest) {
            TCP tcp;
            CHECK(!tcp.is_open());
            string destination = "localhost";
            tcp.connect(destination);
            CHECK(tcp.is_open());
            tcp.close();
            CHECK(!tcp.is_open());
        }
    }
}




#endif	/* TEMPORARYSTATETEST_H */

