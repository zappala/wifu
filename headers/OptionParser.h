#ifndef _option_parser_h
#define	_option_parser_h

// includes
#include <getopt.h>
#include <unistd.h>

// C++ includes
#include <map>

using namespace std;

class OptionParser {
public:
    static OptionParser& instance();

    virtual ~OptionParser();

    void parse(int argc, char* const argv[], const struct option* long_options);

    bool present(const char* option);

    const char* argument(const char* option);

private:

    OptionParser();


    map<const char*, bool> present_;
    map<const char*, const char*> argument_;
};

#endif  // option_parser_h

