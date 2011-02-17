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
    OptionParser();

    virtual ~OptionParser();

    void parse(int, char* const[], const struct option*);

    bool present(const char*);

    const char* argument(const char*);

private:
    map<const char*, bool> present_;
    map<const char*, const char*> argument_;
};

#endif  // option_parser_h

