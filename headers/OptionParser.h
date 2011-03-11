#ifndef _option_parser_h
#define	_option_parser_h

// includes
#include <getopt.h>
#include <unistd.h>

// C++ includes
#include <map>
#include <iostream>

using namespace std;

class OptionParser {
public:
    static OptionParser& instance();

    virtual ~OptionParser();

    void parse(int argc, char* const argv[], const struct option* long_options);

    bool present(string& option);

    string& argument(string& option);

private:

    OptionParser();

    map<string, bool> present_;
    map<string, string> argument_;
};

#endif  // option_parser_h

