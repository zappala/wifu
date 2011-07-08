/* 
 * File:   SourceGetter.h
 * Author: rbuck
 *
 * Created on January 26, 2011, 10:13 AM
 */

#ifndef _SOURCEGETTER_H
#define	_SOURCEGETTER_H

#include <asm/types.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include "AddressPort.h"

using namespace std;

#define BUFSIZE 8192

struct address_info {
    u_int dest_addr;
    u_int src_addr;
    char if_name[IF_NAMESIZE];
};


class SourceGetter {
public:
    ~SourceGetter();
    static SourceGetter& instance();

    gcstring get_source_address(gcstring& dest_address);

private:
    SourceGetter();
    in_addr_t get_bitmask(const char* interface);
    int get_address_info(vector<struct address_info, gc_allocator<struct address_info> >& info);
    int read_response(int sock, char* buffer, int seq);
    void parse_routes(struct nlmsghdr* headder, struct address_info* info);

    // key = destination, value = source
    gcstring_map cache_;
    gcstring_map::iterator cache_itr_;
    int netlink_socket_;
    int udp_socket_;
    vector<struct address_info, gc_allocator<struct address_info> > infos_;

};

#endif	/* _SOURCEGETTER_H */

