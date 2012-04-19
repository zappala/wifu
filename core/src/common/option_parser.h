#ifndef _option_parser_h
#define	_option_parser_h

// includes
#include <getopt.h>
#include <unistd.h>

// C++ includes
#include <map>

// local includes
#include "defines.h"

class OptionParser {

public:

    OptionParser();
    ~OptionParser();

    void parse(int argc, char* const argv[], const struct option* longopts);
    bool present(const char* option);
    const char* argument(const char* option);

private:

    map<const char*, bool> present_;
    map<const char*, const char*> argument_;
};

#endif  // option_parser_h

