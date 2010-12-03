/*
 * test.cc
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */


#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "headers/CheckMacros.h"

#include "WifuEndAPITest.h"
#include "UDPSocketTest.h"
#include "AddressPortTest.h"
#include "SemaphoreTest.h"
#include "BinarySemaphoreTest.h"
#include "IDGeneratorTest.h"
#include "IdentifiableTest.h"
#include "QueueTest.h"
#include "PacketTest.h"

#include "LocalSocketFullDuplexTest.h"
#include "QueryStringParserTest.h"
#include "SocketMapTest.h"
#include "SocketManagerTest.h"

//#include "IntegrationTest.h"

using namespace UnitTest;

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

void cleanup() {
    int value = system("rm LibrarySocket*");
    if (value < 0) {
        perror("Error Removing LibrarySocket Files");
    }

    value = system("rm WifuSocket");
    if (value < 0) {
        perror("Error Removing WifuSocket File");
    }

}

int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;

    change_dir();
    int result = UnitTest::RunAllTests();
    //    cleanup();

    return result;
}