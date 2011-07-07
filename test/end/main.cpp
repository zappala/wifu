/*
 * main.cpp
 *
 *  Created on: Jan 25, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "GarbageCollector.h"
#include "PortManagerFactory.h"
#include "StandardPortManagerCreator.h"

int main(int argc, char** argv) {
    GC_INIT();
    PortManagerFactory::instance().set_creator(new StandardPortManagerCreator());
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
