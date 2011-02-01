/*
 * main.cc
 *
 *  Created on: Dec 27, 2010
 *      Author: erickson
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string>

#include "UnitTest++.h"

#include "headers/WifuEndAPITest.h"
#include "headers/IntegrationTest.h"

using namespace UnitTest;
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
    std::cout << "Running frontend tests" << std::endl;
    change_dir();
    return UnitTest::RunAllTests();
}
