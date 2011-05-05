#include "../applib/wifu_socket.h"
#include "AddressPort.h"
#include "defines.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

    system("cp bin/wifu-end /tmp/");
    chdir("/tmp");
    system("./wifu-end --network standard");
    sleep(1);

    AddressPort to_bind("192.168.21.106", 5000);

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


    while ((connection = wifu_accept(server, (struct sockaddr*) &addr, &length))) {
        cout << "Connection Established" << endl;

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
        break;
    }
    wifu_close(connection);
    wifu_close(server);

    sleep(1);
    system("killall wifu-end");
}
