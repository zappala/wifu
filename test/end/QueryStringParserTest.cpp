/* 
 * File:   QueryStringParser.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 4:33 PM
 */

#ifndef QUERYSTRINGPARSERTEST_H
#define	QUERYSTRINGPARSERTEST_H

#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

#include "UnitTest++.h"
#include "../headers/QueryStringParser.h"

using namespace std;

namespace {

    SUITE(QueryStringParserTest) {
        TEST(QueryStringParserCreateTest) {

            char buf[(sizeof(int)*8+1)];

            string name = "name";
            map<string, string> values;

            string result = QueryStringParser::create(name, values);
            string expected = "name?";
            CHECK_EQUAL(expected, result);

            values["a"] = "a";
            result = QueryStringParser::create(name, values);
            expected = "name?a=a&";
            CHECK_EQUAL(expected, result);

            sprintf(buf, "%d", 95);
            values["a"] = buf;
            result = QueryStringParser::create(name, values);
            expected = "name?a=95&";
            CHECK_EQUAL(expected, result);

            sprintf(buf, "%d", 4);
            values["b"] = buf;
            result = QueryStringParser::create(name, values);
            expected = "name?a=95&b=4&";
            CHECK_EQUAL(expected, result);
        }

        TEST(QueryStringParserParseTest) {

            map<string, string> values;
            string query = "name?a=9&b=bob&c=1234&";

            QueryStringParser::parse(query, values);

            string expected = "name";
            CHECK_EQUAL(expected, values["name"]);

            expected = "9";
            CHECK_EQUAL(expected, values["a"]);

            expected = "1234";
            CHECK_EQUAL(expected, values["c"]);

            expected = "bob";
            CHECK_EQUAL(expected, values["b"]);
        }
    }
}

#endif	/* QUERYSTRINGPARSERTEST_H */

