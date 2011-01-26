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
#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include "../headers/AddressPort.h"

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

    string get_source_address(string& dest_address);

private:
    SourceGetter();

    in_addr_t get_bitmask(const char* interface);

    int get_address_info(vector<address_info>& info);

    int read_response(int sock, char* buffer, int seq);

    void parse_routes(struct nlmsghdr* headder, struct address_info* info);

    // key = destination, value = source
    map<string, string> cache_;
    map<string, string>::iterator cache_itr_;
    int netlink_socket_;
    int udp_socket_;
    vector<struct address_info> infos_;

};

#endif	/* _SOURCEGETTER_H */

