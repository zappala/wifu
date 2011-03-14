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

        char* args[4] = {(char*) "./awesome", (char*) "--network", (char*) "standard", NULL};

        parser.parse(3, args, long_options);

        string network = "network";
        string standard = "standard";


        ASSERT_TRUE(parser.present(network));
        ASSERT_EQ(standard, parser.argument(network));
    }

}

#endif /* OPTIONPARSERTEST_H_ */
