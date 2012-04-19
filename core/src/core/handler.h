// A generic class for WiFu packet handlers.  Each handler should subclass
// this class and override its methods with the desired functionality.

#ifndef _handler_h
#define _handler_h

#include <arpa/inet.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/netfilter.h>
#include <iostream>
#include <fstream>
#include <semaphore.h>

#include "common/utils.h"
#include "packet/ippacket.h"

class Handler {

public:

    Handler();
    virtual ~Handler();

    // parse command-line options
    virtual void parse_options(int argc, char* const argv[]);

    // methods to handle packets coming via the input, output, forwarding,
    // prerouting, or postrouting iptables
    // 
    // p: IPPacket ready for processing (mangling, accept, drop, etc.)
    //
    virtual void input(IPPacket& p);
    virtual void output(IPPacket& p);
    virtual void forward(IPPacket& p);
    virtual void prerouting(IPPacket& p);
    virtual void postrouting(IPPacket& p);

    // start any threads that need to be running
    virtual void start();
};

#endif // _handler_h
