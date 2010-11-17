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


using namespace UnitTest;

int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;
    return UnitTest::RunAllTests();
}