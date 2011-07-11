#include "QueryStringParser.h"

void QueryStringParser::parse(gcstring& message, gcstring_map& values) {
    size_t end = message.find_first_of('?', 0);
    if (end == gcstring::npos) {
        throw WiFuException("Malformed query");
    }

    gcstring name = message.substr(0, end);

    values[NAME_STRING] = name;
    gcstring key;
    gcstring value;
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
        value = message.substr(begin, end - begin);

        values[key] = value;
        // TODO: could we make it a bit better (faster) by putting the data at the end of the message?
    }
}

void QueryStringParser::create(gcstring name, gcstring_map& values, gcstring& message) {
    gcstring_map::iterator itr;
    message = name + '?';
    for (itr = values.begin(); itr != values.end(); ++itr) {
        message.append(itr->first + '=' + itr->second + '&');
    }
}
