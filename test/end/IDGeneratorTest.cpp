/* 
 * File:   IDGeneratorTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 3:50 PM
 */

#ifndef _IDGENERATORTEST_H
#define	_IDGENERATORTEST_H

#include "../headers/IDGenerator.h"
#include "../headers/PortManager.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

namespace {
	TEST(IDGeneratorTest) {
		UNITTEST_TIME_CONSTRAINT(1);
		int id0 = IDGenerator::instance().get();
		int id1 = IDGenerator::instance().get();

		ASSERT_TRUE(id0 != id1);

		IDGenerator::instance().remove(id0);

	}
}

#endif	/* _IDGENERATORTEST_H */

