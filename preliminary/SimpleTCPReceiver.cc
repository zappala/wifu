#include "../applib/wifu_socket.h"
#include "AddressPort.h"
#include "defines.h"
#include <iostream>
#include "OptionParser.h"
#include <stdlib.h>
#include <bits/basic_string.h>
#include "Timer.h"
#include "headers/ISocketAPI.h"
#include "headers/WiFuSocketAPI.h"
#include "headers/KernelSocketAPI.h"
#include <errno.h>

using namespace std;

#define optionparser OptionParser::instance()

int main(int argc, char** argv) {

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";

    gcstring portarg = "port";
    int port = 5002;

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = TCP_TAHOE;

    static struct option long_options[] = {
        {bindaddr.c_str(), required_argument, NULL, 0},
        {portarg.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        {protocolarg.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(bindaddr)) {
        hostaddr = optionparser.argument(bindaddr);
    } else {
        cout << "Binding address required!\n";
        cout << "Use option --address <addr>\n";
        return -1;
    }
    if (optionparser.present(portarg)) {
        port = atoi(optionparser.argument(portarg).c_str());
    }

    if (optionparser.present(apiarg)) {
        gcstring api_type = optionparser.argument(apiarg);
        if (!api_type.compare("kernel")) {
            api = new KernelSocketAPI();
        }
    }

    if (optionparser.present(protocolarg)) {
        protocol = atoi(optionparser.argument(protocolarg).c_str());
    }

    cout << bindaddr << " " << hostaddr << endl;
    cout << portarg << " " << port << endl;
    cout << apiarg << " " << api->get_type() << endl;
    cout << protocolarg << " " << protocol << endl;

    AddressPort to_bind(hostaddr, port);

    int server = api->custom_socket(AF_INET, SOCK_STREAM, protocol);
    if (server <= 0) {
        perror("socket");
    }

    int result = api->custom_bind(server, (const struct sockaddr*) to_bind.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    if (result < 0) {
        perror("bind");
    }

    result = api->custom_listen(server, 5);
    if (result < 0) {
        perror("listen");
    }

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;

    while ((connection = api->custom_accept(server, (struct sockaddr*) &addr, &length)) > 0) {
        AddressPort remote(&addr);
        cout << "Connection Established to: " << remote.to_s() << endl;

        int size = 50000;
        char buffer[size];
        gcstring all_received = "";
        int num_received = 0;

        Timer recv_timer;
        while (true) {
            //memset(buffer, 0, size);
            int return_value = api->custom_recv(connection, buffer, 30000, 0);
            recv_timer.start();

            if (return_value == 0) {
                break;
            }
            //all_received.append(buffer);
            num_received += return_value;
        }
        recv_timer.stop();
        api->custom_close(connection);
        cout << "Duration (us) to recv " << num_received << " bytes from " << remote.to_s() << ": " << recv_timer.get_duration_microseconds() << endl;

    }

    api->custom_close(server);

    sleep(1);
}
