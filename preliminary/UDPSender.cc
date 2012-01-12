/* 
 * File:   UDPSender.cc
 * Author: rbuck
 *
 * Created on January 11, 2012, 2:55 PM
 */

#include <cstdlib>

#include "headers/ISocketAPI.h"
#include "headers/WiFuSocketAPI.h"
#include "headers/KernelSocketAPI.h"
#include "OptionParser.h"
#include "AddressPort.h"
#include "Timer.h"

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}
using namespace std;

#define optionparser OptionParser::instance()

/*
 * 
 */
int main(int argc, char** argv) {


    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring portarg = "port";
    int port = 5002;

    gcstring protocolarg = "protocol";
    int protocol = UDP;

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";

    static struct option long_options[] = {
        {bindaddr.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        {portarg.c_str(), required_argument, NULL, 0},
        {protocolarg.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(apiarg)) {
        gcstring api_type = optionparser.argument(apiarg);
        if (!api_type.compare("kernel")) {
            api = new KernelSocketAPI();
        }
    }

    if (optionparser.present(portarg)) {
        port = atoi(optionparser.argument(portarg).c_str());
    }

    if (optionparser.present(protocolarg)) {
        protocol = atoi(optionparser.argument(protocolarg).c_str());
    }

    if (optionparser.present(bindaddr)) {
        hostaddr = optionparser.argument(bindaddr);
    } else {
        cout << "Binding address required!\n";
        cout << "Use option --address <addr>\n";
        return -1;
    }

    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    sock = api->custom_socket(AF_INET, SOCK_DGRAM, protocol);
    if (sock < 0) error("Opening socket");
    length = sizeof (server);
    bzero(&server, length);
    AddressPort ap(hostaddr, port);
    if (api->custom_bind(sock, (struct sockaddr *) ap.get_network_struct_ptr(), length) < 0) {
        error("binding");
    }
    fromlen = sizeof (struct sockaddr_in);
    while (1) {
        n = api->custom_recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &from, &fromlen);
        if (n < 0) error("recvfrom");
        n = api->custom_sendto(sock, "Got your message\n", 17, 0, (struct sockaddr *) &from, fromlen);
        if (n < 0) error("sendto");
    }
    return 0;
}

