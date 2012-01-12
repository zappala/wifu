/* 
 * File:   UDPReceiver.cc
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
/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
using namespace std;

#define optionparser OptionParser::instance()

/*
 * 
 */
int main(int argc, char** argv) {

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring destination = "destination";
    gcstring dest = "127.0.0.1";
    
    gcstring destport = "port";
    int port = 5002;

    gcstring protocolarg = "protocol";
    int protocol = UDP;

    static struct option long_options[] = {
        {apiarg.c_str(), required_argument, NULL, 0},
        {destination.c_str(), required_argument, NULL, 0},
        {destport.c_str(), required_argument, NULL, 0},
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
    if (optionparser.present(destination)) {
        dest = optionparser.argument(destination);
    } else {
        cout << "Destination argument required!\n";
        cout << "Use option --destination <addr>\n";
        return -1;
    }
    if (optionparser.present(destport)) {
        port = atoi(optionparser.argument(destport).c_str());
    }

    if (optionparser.present(protocolarg)) {
        protocol = atoi(optionparser.argument(protocolarg).c_str());
    }

    int sock, n;
    unsigned int length;
    struct sockaddr_in server, from;

    sock = api->custom_socket(AF_INET, SOCK_DGRAM, protocol);
    if (sock < 0) error("socket");

    AddressPort ap(dest, port);

    length = sizeof (struct sockaddr_in);
    gcstring message = "a";

    char buffer[255];
    n = api->custom_sendto(sock, message.data(), message.size(), 0, (const struct sockaddr *) ap.get_network_struct_ptr(), length);
    if (n < 0) error("Sendto");

    n = api->custom_recvfrom(sock, buffer, 256, 0, (struct sockaddr *) &from, &length);
    if (n < 0) error("recvfrom");
    cout << buffer << endl;
    api->custom_close(sock);
    return 0;
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}