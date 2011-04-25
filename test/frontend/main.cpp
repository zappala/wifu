/*
 * main.cc
 *
 *  Created on: Dec 27, 2010
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include <string>
#include <signal.h>

using namespace std;

void change_dir() {
    int size = 1000;
    char buf[size];
    getcwd(buf, size);
    string path = buf;
    string bin = "bin";

    if (path.find(bin) == string::npos) {
        chdir(bin.c_str());
    }
}


int main(int argc, char** argv) {
    change_dir();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
