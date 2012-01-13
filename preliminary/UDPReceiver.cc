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
#include "Timer.h"
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

ISocketAPI* api;
int sock;
Timer t;
int total;

void main_signal_manager(int sig) {
    switch (sig) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            api->custom_close(sock);
            cout << "Duration to receive " << total << " bytes: " << t.get_duration_microseconds() << endl;
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
}

void register_signals() {
    signal(SIGINT, main_signal_manager);
    signal(SIGQUIT, main_signal_manager);
    signal(SIGTERM, main_signal_manager);
}

/*
 * 
 */
int main(int argc, char** argv) {

    register_signals();

    gcstring apiarg = "api";
    api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = UDP;

    gcstring destport = "port";
    int port = 5002;

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";

    static struct option long_options[] = {
        {bindaddr.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        
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
    
    if (optionparser.present(destport)) {
        port = atoi(optionparser.argument(destport).c_str());
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

    int n;
    unsigned int length;
    struct sockaddr_in from;

    int size = 1472;
    char buffer[size];
    total = 0;

    AddressPort ap(hostaddr, port);
    length = sizeof (struct sockaddr_in);

    sock = api->custom_socket(AF_INET, SOCK_DGRAM, protocol);
    if (sock < 0) error("socket");

    length = sizeof (struct sockaddr_in);
    if (api->custom_bind(sock, (struct sockaddr *) ap.get_network_struct_ptr(), length) < 0) {
        error("binding");
    }
    
    while (1) {
        t.update_stop();
        n = api->custom_recvfrom(sock, buffer, size, 0, (struct sockaddr *) &from, &length);
        t.start();

        if (n < 0) error("recvfrom");
        total += n;

    }

    return 0;
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}