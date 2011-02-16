/*
 * main.cpp
 *
 *  Created on: Jan 25, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "GarbageCollector.h"

int main(int argc, char** argv) {
    GC_INIT();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
