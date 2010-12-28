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

#include "UnitTest++.h"

#include "WifuEndAPITest.h"
#include "IntegrationTest.h"

using namespace UnitTest;

int main(int argc, char** argv) {
    std::cout << "Running frontend tests" << std::endl;
    return UnitTest::RunAllTests();
}
