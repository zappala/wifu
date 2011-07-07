/* 
 * File:   QueryStringParser.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 4:20 PM
 */

#ifndef QUERYSTRINGPARSER_H
#define	QUERYSTRINGPARSER_H

#include <sstream>
#include <map>
#include <iostream>
#include "exceptions/WiFuException.h"

#include "defines.h"

using namespace std;

class QueryStringParser {
public:
    static void parse(gcstring& message, map<gcstring, gcstring>& values);

    static gcstring create(gcstring name, map<gcstring, gcstring>& values);
};

#endif	/* QUERYSTRINGPARSER_H */

