//#include "../applib/wifu_socket.h"

#include <iostream>
#include "../applib/wifu_socket.h"
#include "defines.h"
#include "AddressPort.h"

using namespace std;

int main(int argc, char** argv) {

    system("cp bin/wifu-end /tmp/");
    chdir("/tmp");
    system("./wifu-end --network standard");
    sleep(1);

    AddressPort to_connect("192.168.21.106", 5000);
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
