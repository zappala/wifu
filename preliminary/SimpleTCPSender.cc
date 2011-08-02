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
#include "headers/ISocketAPI.h"
#include "headers/WiFuSocketAPI.h"
#include "headers/KernelSocketAPI.h"


using namespace std;

#define optionparser OptionParser::instance()

int main(int argc, char** argv) {

    gcstring destination = "destination";
    gcstring destport = "port";
    gcstring dest = "127.0.0.1";
    gcstring num = "num";
    int port = 5002;

    gcstring apiarg = "api";
    ISocketAPI* api = new WiFuSocketAPI();

    gcstring protocolarg = "protocol";
    int protocol = TCP_TAHOE;

    gcstring chunkarg = "chunk";
    // The number of bytes we are willing to receive each time we call recv().
    int chunk = 10000;

    static struct option long_options[] = {
        {destination.c_str(), required_argument, NULL, 0},
        {destport.c_str(), required_argument, NULL, 0},
        {num.c_str(), required_argument, NULL, 0},
        {apiarg.c_str(), required_argument, NULL, 0},
        {protocolarg.c_str(), required_argument, NULL, 0},
        {chunkarg.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
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

    if(optionparser.present(apiarg)) {
        gcstring api_type = optionparser.argument(apiarg);
        if(!api_type.compare("kernel")) {
            api = new KernelSocketAPI();
        }
    }

    if(optionparser.present(protocolarg)) {
        protocol = atoi(optionparser.argument(protocolarg).c_str());
    }

    if(optionparser.present(chunkarg)) {
        chunk = atoi(optionparser.argument(chunkarg).c_str());
    }

    cout << destination << " " << dest << endl;
    cout << destport << " " << port << endl;
    cout << apiarg << " " << api->get_type() << endl;
    cout << protocolarg << " " << protocol << endl;
    cout << num << " " << num_bytes << endl;
    cout << chunkarg << " " << chunk << endl;

    gcstring message = RandomStringGenerator::get_data(num_bytes);

    AddressPort to_connect(dest, port);
    int client = api->custom_socket(AF_INET, SOCK_STREAM, protocol);
    int result = api->custom_connect(client, (const struct sockaddr*) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    assert(!result);

    int index = 0;
    int num_sent = 0;

    Timer send_timer;
    send_timer.start();

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

        int sent = api->custom_send(client, data, chunk, 0);
        num_sent += sent;
        index += sent;
    }
    send_timer.stop();
    cout << "Duration (us) to send " << message.size() << " bytes to " << to_connect.to_s() << ": " << send_timer.get_duration_microseconds() << endl;
    api->custom_close(client);

    sleep(1);
}
