/* 
 * File:   QueryStringParser.h
 * Author: rbuck
 *
 * Created on November 23, 2010, 4:20 PM
 */

#ifndef QUERYSTRINGPARSER_H
#define	QUERYSTRINGPARSER_H

#include <map>
#include <iostream>
#include "exceptions/WiFuException.h"

#include "defines.h"

using namespace std;

class QueryStringParser {
public:
    static void parse(gcstring& message, gcstring_map& values);

    static void create(gcstring name, gcstring_map& values, gcstring& message);
};

#endif	/* QUERYSTRINGPARSER_H */

