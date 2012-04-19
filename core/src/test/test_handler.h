// Shim handler -- tests the insertion of a shim at output and removal
// on input

#ifndef _test_handler_h
#define _test_handler_h

// includes
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

// local includes
#include "common/logger.h"
#include "common/option_parser.h"
#include "core/handler.h"
#include "packet/packet.h"
#include "packet/ippacket.h"
#include "packet/tcppacket.h"
#include "packet/udppacket.h"

struct test_shim {
    u_int32_t a;
};

class TestHandler : public Handler {

public:
    TestHandler();
    ~TestHandler();

    // parse command-line options
    void parse_options(int argc, char* const argv[]);

    // methods to handle packets coming via the input, output, or forwarding
    // IPtables chains
    void input(IPPacket& p);
    void output(IPPacket& p);
    void forward(IPPacket& p);

    // print a packet
    void print_packet(Packet* p);

private:
    void testing(TCPPacket * p);
    Logger* logger_;
    bool logging_;
    bool insert_shim_;
};


#endif // _shim_h
