#include "OptionParser.h"

OptionParser& OptionParser::instance() {
    static OptionParser instance_;
    return instance_;
}

OptionParser::OptionParser() {
}

OptionParser::~OptionParser() {
}

void OptionParser::parse(int argc, char* const argv[], const struct option* long_options) {
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
                if (long_options[index].has_arg != no_argument) {
                    argument_[long_options[index].name] = optarg;
                }
        }
    }
}

bool OptionParser::present(string& option) {
    return present_[option];
}

string& OptionParser::argument(string& option) {
    return argument_[option];
}
