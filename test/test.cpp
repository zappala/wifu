/*
 * test.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */

#include "UnitTest++.h"
#include <iostream>
#include <string>
#include "../headers/AddressPort.h"
#include "headers/CheckMacros.h"
#include <vector>

using namespace UnitTest;
using namespace std;

int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;
    return UnitTest::RunAllTests();
}

namespace {

    TEST(AddresPortTest) {
        string address("address");
        string port("port");
        AddressPort a(address, port);
        CHECK_EQUAL(a.get_address(), address);
        CHECK_EQUAL(a.get_port(), port);
        CHECK_EQUAL(4, 4);

    }

    TEST(ValidCheckSucceeds) {
        bool const b = true;
        CHECK(b);

    }

    TEST(CheckWorksWithPointers) {
        void* p = (void *) 0x100;
        CHECK(p);
        CHECK(p != 0);
    }

    TEST(ValidCheckEqualSucceeds) {
        int const x = 3;
        int const y = 3;
        CHECK_EQUAL(x, y);
    }

    TEST(CheckEqualWorksWithPointers) {
        void* p = (void *) 0;
        CHECK_EQUAL((void*) 0, p);
    }

    TEST(ValidCheckCloseSucceeds) {
        CHECK_CLOSE(2.0f, 2.001f, 0.01f);
        CHECK_CLOSE(2.001f, 2.0f, 0.01f);
    }

    TEST(ArrayCloseSucceeds) {
        float const a1[] = {1, 2, 3};
        float const a2[] = {1, 2.01f, 3};
        CHECK_ARRAY_CLOSE(a1, a2, 3, 0.1f);
    }

    TEST(CheckArrayCloseWorksWithVectors) {
        std::vector< float > a(4);
        for (int i = 0; i < 4; ++i)
            a[i] = (float) i;

        CHECK_ARRAY_CLOSE(a, a, (int) a.size(), 0.0001f);
    }

    

    void CheckBool(const bool b) {
        CHECK(b);
    }

    TEST(CanCallCHECKOutsideOfTestFunction) {
        CheckBool(true);
    }

}