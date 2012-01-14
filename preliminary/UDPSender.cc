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
#include "packet/UDPPacket.h"
#include "../test/headers/RandomStringGenerator.h"

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

    gcstring destination = "destination";
    gcstring dest = "127.0.0.1";
    

    static struct option long_options[] = {
        {destination.c_str(), required_argument, NULL, 0},
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

    if (optionparser.present(destination)) {
        dest = optionparser.argument(destination);
    } else {
        cout << "Destination argument required!\n";
        cout << "Use option --destination <addr>\n";
        return -1;
    }

   

    int sock, n;

    sock = api->custom_socket(AF_INET, SOCK_DGRAM, protocol);
    if (sock < 0) error("Opening socket");

    int length = 1472;
    gcstring message = RandomStringGenerator::get_data(length);

    AddressPort receiver(dest, port);
    socklen_t size = sizeof(struct sockaddr_in);

    int total = 0;

    Timer t;
    t.start();
    for (int i = 0; i < 100000; ++i) {
        n = api->custom_sendto(sock, message.data(), length, 0, (struct sockaddr *) receiver.get_network_struct_ptr(), size);
        if (n < 0) error("recvfrom");
        total += n;
    }
    t.stop();

    cout << "Time to send " << total << " bytes: " << t.get_duration_microseconds() << endl;

    return 0;
}

