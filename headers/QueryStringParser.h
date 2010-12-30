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
    static void parse(string& message, map<string, string>& values) {
        size_t end = message.find_first_of('?', 0);
        string name = message.substr(0, end);

        values[NAME_STRING] = name;
        string key;
        string value;
        size_t begin;


        while(1) {
            // Find Key
            begin = end + 1;

            if(begin >= message.size()) {
                break;
            }

            end = message.find_first_of('=', begin);
            key = message.substr(begin, end - begin);

            // Find Value
            begin = end + 1;
            end = message.find_first_of('&', begin);
            value = message.substr(begin, end - begin);

            values[key] = value;
        }
    }

    static string create(string name, map<string, string>& values) {
        stringstream s;
        s << name << "?";
        map<string, string>::iterator itr;
        for(itr = values.begin(); itr != values.end(); ++itr) {
            s << itr->first << "=" << itr->second << "&";
        }
        return s.str();
    }
};

#endif	/* QUERYSTRINGPARSER_H */

