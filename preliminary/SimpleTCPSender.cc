//#include "../applib/wifu_socket.h"

#include <iostream>
#include <stdlib.h>
#include <bits/basic_string.h>
#include "../applib/wifu_socket.h"
#include "defines.h"
#include "AddressPort.h"
#include "OptionParser.h"

using namespace std;

#define optionparser OptionParser::instance()

int main(int argc, char** argv) {

    string destination = "destination";
    string destport = "port";
    string dest = "127.0.0.1";
    int port = 5002;

    static struct option long_options[] = {
        {destination.c_str(), required_argument, NULL, 0},
        {destport.c_str(), required_argument, NULL, 0},
        {0, 0}
    };
    
    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(destination)) {
        dest = optionparser.argument(destination);
    }
    else {
        cout << "Destination argument required!\n";
        cout << "Use option --destination <addr>\n";
        return -1;
    }
    if (optionparser.present(destport)) {
        port = atoi(optionparser.argument(destport).c_str());
    }

    system("cp wifu-end /tmp/");
    chdir("/tmp");
    system("./wifu-end --network standard");
    sleep(1);

    AddressPort to_connect(dest, port);
    int client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    int result = wifu_connect(client, (const struct sockaddr*) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    assert(!result);

    int size = 50000;
    char buffer[size];

    string message = "";
    while (message.compare("stop")) {
        for (int i = 0; i < 10000; i++) {
            message = "This is a message\n";
            wifu_send(client, message.c_str(), message.size(), 0);
        }
        message = "stop";
    }
    cout << "Done sending" << endl;
    wifu_close(client);

    sleep(1);
    system("killall wifu-end");
}
