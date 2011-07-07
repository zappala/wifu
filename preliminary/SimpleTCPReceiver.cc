#include "../applib/wifu_socket.h"
#include "AddressPort.h"
#include "defines.h"
#include <iostream>
#include "OptionParser.h"
#include <stdlib.h>
#include <bits/basic_string.h>

using namespace std;

#define optionparser OptionParser::instance()

int main(int argc, char** argv) {

    gcstring bindaddr = "address";
    gcstring hostaddr = "127.0.0.1";
    gcstring portarg = "port";
    int port = 5002;

    static struct option long_options[] = {
        {bindaddr.c_str(), required_argument, NULL, 0},
        {portarg.c_str(), required_argument, NULL, 0},
        {0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(bindaddr)) {
        hostaddr = optionparser.argument(bindaddr);
    }
    else {
        cout << "Binding address required!\n";
        cout << "Use option --address <addr>\n";
        return -1;
    }
    if (optionparser.present(portarg)) {
        port = atoi(optionparser.argument(portarg).c_str());
    }
    
    system("cp wifu-end /tmp/");
    chdir("/tmp");
    system("./wifu-end --network standard");
    sleep(1);

    AddressPort to_bind(hostaddr, port);

    int server = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    assert(server > 0);

    int result = wifu_bind(server, (const struct sockaddr*) to_bind.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    assert(!result);

    result = wifu_listen(server, 5);
    assert(!result);

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    int size = 5000;
    char buffer[size];


    while ((connection = wifu_accept(server, (struct sockaddr*) &addr, &length)) > 0) {
        AddressPort remote(&addr);
        cout << "Connection Established to: " << remote.to_s() << endl;

        int count = 0;
        while (true) {
            memset(buffer, 0, size);
            int received = wifu_recv(connection, buffer, size, 0);
            if (received <= 0) {
                cout << "Breaking" << endl;
                break;
            }
            cout << "Message# " << count++ << ": " << buffer << endl;
        }
        wifu_close(connection);
    }
    
    wifu_close(server);

    sleep(1);
    system("killall wifu-end");
}
