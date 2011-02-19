/* 
 * File:   QueryStringParser.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 4:20 PM
 */

#ifndef QUERYSTRINGPARSER_H
#define	QUERYSTRINGPARSER_H

#include <string>
#include <sstream>
#include <map>

#include "defines.h"

using namespace std;

class QueryStringParser {
public:
    static void parse(string& message, map<string, string>& values);

    static string create(string name, map<string, string>& values);
};

#endif	/* QUERYSTRINGPARSER_H */

