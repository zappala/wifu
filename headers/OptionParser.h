#ifndef _option_parser_h
#define	_option_parser_h

// includes
#include <getopt.h>
#include <unistd.h>

// C++ includes
#include <map>
#include <iostream>

#include "defines.h"

using namespace std;

class OptionParser {
public:
    static OptionParser& instance();

    virtual ~OptionParser();

    void parse(int argc, char* const argv[], const struct option* long_options);

    bool present(gcstring& option);

    gcstring& argument(gcstring& option);

private:

    OptionParser();

    map<gcstring, bool> present_;
    map<gcstring, gcstring> argument_;
};

#endif  // option_parser_h

