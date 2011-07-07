/* 
 * File:   QueryStringParser.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 4:33 PM
 */

#ifndef QUERYSTRINGPARSERTEST_H
#define	QUERYSTRINGPARSERTEST_H

#include <iostream>
#include <map>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "QueryStringParser.h"

using namespace std;

namespace {

    TEST(QueryStringParserTest, Create) {

        char buf[(sizeof (int) *8 + 1)];

        gcstring name = "name";
        map<gcstring, gcstring> values;

        gcstring result = QueryStringParser::create(name, values);
        gcstring expected = "name?";
        ASSERT_EQ(expected, result);

        values["a"] = "a";
        result = QueryStringParser::create(name, values);
        expected = "name?a=a&";
        ASSERT_EQ(expected, result);

        sprintf(buf, "%d", 95);
        values["a"] = buf;
        result = QueryStringParser::create(name, values);
        expected = "name?a=95&";
        ASSERT_EQ(expected, result);

        sprintf(buf, "%d", 4);
        values["b"] = buf;
        result = QueryStringParser::create(name, values);
        expected = "name?a=95&b=4&";
        ASSERT_EQ(expected, result);
    }

    TEST(QueryStringParserTest, moreCreate) {

        map<gcstring, gcstring> values;
        gcstring query = "name?a=9&b=bob&c=1234&";

        QueryStringParser::parse(query, values);

        gcstring expected = "name";
        ASSERT_EQ(expected, values["name"]);

        expected = "9";
        ASSERT_EQ(expected, values["a"]);

        expected = "1234";
        ASSERT_EQ(expected, values["c"]);

        expected = "bob";
        ASSERT_EQ(expected, values["b"]);
    }
}

#endif	/* QUERYSTRINGPARSERTEST_H */

