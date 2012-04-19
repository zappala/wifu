#ifndef _packet_h
#define	_packet_h

// includes
#include <arpa/inet.h>
#include <assert.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/netfilter.h>
#include <stdlib.h>
#include <string.h>

// C++ includes
#include <string>
#include <sstream>

// local includes
#include "common/defines.h"

// Provides a mapping to the filter a packet arrived on
enum Filter {
    input = 0,
    output = 1,
    forward = 2,
    prerouting = 3,
    postrouting = 4
};

// A generic packet class.  All memory management is done internally.
class Packet {

public:
    // Constructor that sets the following class variables:
    //
    // qh: netfilter queue handle
    // id: netfilter id
    // payload_length: size in bytes of payload
    // payload: pointer to the buffer containing an ip packet
    // filter: enum type of which queue this Packet came from
    // preserve_payload: if true, make a copy of the payload
    Packet(struct nfq_q_handle* qh, int id, int payload_length,
           unsigned char* payload, Filter filter, bool preserve_payload=true);

    // Constructor that copies the contents of a packet into this packet
    //
    // p: packet to copy into this packet
    // preserve_payload: if true, make a copy of the payload
    Packet(Packet& p, bool preserve_payload=true);

    virtual ~Packet();

    // Gets ID for this packet
    int id();

    // Delivers this packet back to the kernel for further processing
    void accept();

    // Delivers this packet back to the kernel for further processing
    //
    // @use_payload: if true, tell the kernel we have modified the packet
    void accept(bool use_payload);

    // Tells the kernel to drop the packet
    void drop();

    // Gets the length of the internal payload buffer
    int get_payload_length();

    // Gets the filter type of this Packet
    Filter get_filter();

    // Gets a string representation of the filter type of this Packet
    string get_filter_s();

    // Boolean methods that tell which packet a filter arrived on 
    bool is_output();
    bool is_input();
    bool is_forward();
    bool is_prerouting();
    bool is_postrouting();

    // Provides string representation of a packet. See to_s_format for
    // a definition of what is actually returned.
    virtual string to_s();

    // Format of the string version of a packet
    virtual string to_s_format();
    
protected:

    // Netfilter queue handle for processing the payload
    struct nfq_q_handle* qh_;

    // Netfilter id for this this payload
    int id_;

    // Length in bytes of payload
    int payload_length_;

    // Pointer to a buffer containing an IP packet
    unsigned char* payload_;

    // The filter this packet came from
    Filter filter_;

private:

    // Sets the verdict of payload
    //
    // verdict: One of the following values:
    //          NF_DROP 0
    //          NF_ACCEPT 1
    //          NF_STOLEN 2
    //          NF_QUEUE 3
    //          NF_REPEAT 4
    //          NF_STOP 5
    // use_payload: If true, use the payload in processing. Setting
    //              this to false will make processing a little faster,
    //              but you will loose any changes you make to the packet.
    void set_verdict(int verdict, bool use_payload);

    // Copies the payload of a packet, if desired
    void copy_payload();

    // If true, copy the payload onto the heap.
    bool preserve_payload_;
};

#endif	// _packet_h
