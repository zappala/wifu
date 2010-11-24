/*
 * test.cc
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */


#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "headers/CheckMacros.h"

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
#include "WifuEndAPITest.h"
#include "SocketMapTest.h"

using namespace UnitTest;

int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;
    return UnitTest::RunAllTests();
}