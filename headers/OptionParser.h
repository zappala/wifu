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

    OptionParser() {

    }

    virtual ~OptionParser() {

    }

    void parse(int argc, char* const argv[], const struct option* long_options) {


        optind = 1;
        opterr = 0;
        int c;
        while (1) {
            // getopt_long stores the option index here
            int index = -1;

            c = getopt_long(argc, argv, "-", long_options, &index);

            // detect the end of the options
            if (c == -1) {
                break;
            }

            if (index == -1)
                continue;

            switch (c) {
                case '?':
                    break;
                default:
                    present_[long_options[index].name] = true;
                    if (long_options[index].has_arg != no_argument)
                        argument_[long_options[index].name] = optarg;
            }
        }


    }

    bool present(const char* option) {
        return present_[option];
    }

    const char* argument(const char* option) {
        return argument_[option];
    }

private:

    map<const char*, bool> present_;
    map<const char*, const char*> argument_;
};

#endif  // option_parser_h

