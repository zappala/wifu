//#include "../applib/wifu_socket.h"

#include <iostream>
#include <stdlib.h>
#include <bits/basic_string.h>
#include "../applib/wifu_socket.h"
#include "defines.h"
#include "AddressPort.h"
#include "OptionParser.h"
#include "../test/headers/RandomStringGenerator.h"
#include "Timer.h"

using namespace std;

#define optionparser OptionParser::instance()

int main(int argc, char** argv) {

    gcstring destination = "destination";
    gcstring destport = "port";
    gcstring dest = "127.0.0.1";
    gcstring num = "num";
    int port = 5002;

    static struct option long_options[] = {
        {destination.c_str(), required_argument, NULL, 0},
        {destport.c_str(), required_argument, NULL, 0},
        {num.c_str(), required_argument, NULL, 0},
        {0, 0}
    };

    optionparser.parse(argc, argv, long_options);
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
    int num_bytes = 1000;
    if(optionparser.present(num)) {
        num_bytes = atoi(optionparser.argument(num).c_str());
    }

//    system("cp wifu-end /tmp/");
//    chdir("/tmp");
//    system("./wifu-end --network standard");
//    sleep(1);

    gcstring message = RandomStringGenerator::get_data(num_bytes);

    AddressPort to_connect(dest, port);
    //int client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    int client = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    int result = wifu_connect(client, (const struct sockaddr*) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    assert(!result);

    int index = 0;
    int chunk = 10000;
    int num_sent = 0;

    Timer send_timer;
    send_timer.start();

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

        int sent = wifu_send(client, data, chunk, 0);
        num_sent += sent;
        index += sent;
    }
    send_timer.stop();
    cout << "Duration (us) to send " << message.size() << " bytes to " << to_connect.to_s() << ": " << send_timer.get_duration_microseconds() << endl;
    wifu_close(client);

    sleep(1);
//    system("killall wifu-end");
}
