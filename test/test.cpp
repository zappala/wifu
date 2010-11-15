/*
 * test.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */

#include "UnitTest++.h"
#include <iostream>

using namespace UnitTest;

int main (int argc, char** argv) {
	std::cout << "Running tests" << std::endl;
	return UnitTest::RunAllTests();
}

TEST(FailSpectacularly)
{
    CHECK(true);
}
