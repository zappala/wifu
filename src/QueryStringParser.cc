#include "QueryStringParser.h"

void QueryStringParser::parse(gcstring& message, gcstring_map& values) {
    size_t end = message.find_first_of('?', 0);
    if (end == gcstring::npos) {
        throw WiFuException("Malformed query");
    }

    values.insert(pair<gcstring, gcstring>(NAME_STRING, message.substr(0, end)));
    gcstring key;
    size_t begin;

    while (1) {
        // Find Key
        begin = end + 1;

        if (begin >= message.size()) {
            break;
        }

        end = message.find_first_of('=', begin);
        key = message.substr(begin, end - begin);

        // Find Value
        begin = end + 1;
        end = message.find_first_of('&', begin);
        values.insert(pair<gcstring, gcstring>(key, message.substr(begin, end - begin)));
    }
}

void QueryStringParser::create(gcstring name, gcstring_map& values, gcstring& message) {
    gcstring_map::iterator itr;
    message = name + '?';
    for (itr = values.begin(); itr != values.end(); ++itr) {
        message.append(itr->first + '=' + itr->second + '&');
    }
}
