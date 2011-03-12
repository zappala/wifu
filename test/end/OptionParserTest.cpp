/*
 * OptionParserTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef OPTIONPARSERTEST_H_
#define OPTIONPARSERTEST_H_

#include "gtest/gtest.h"
#include "OptionParser.h"
#include <string>

using namespace std;

#define parser OptionParser::instance()

namespace {

	static struct option long_options[] = {
		{"network", required_argument, NULL, 0},
		{"mockfile", required_argument, NULL, 0},
		{0, 0, 0, 0}
	};

	TEST(OptionParser, all) {
		string networkOption = "--network=standard";
		char* const args[3] = {"./awesome", const_cast<char*>(networkOption.c_str()), NULL};

		parser.parse(2, args, long_options);

		ASSERT_EQ(true, parser.present(networkOption));
	}

}

#endif /* OPTIONPARSERTEST_H_ */
