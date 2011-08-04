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
#include "Utils.h"

#include <list>

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
    if (optionparser.present(num)) {
        num_bytes = atoi(optionparser.argument(num).c_str());
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

    if (optionparser.present(chunkarg)) {
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

    list<u_int64_t, gc_allocator<u_int64_t> > starts, ends;
    list<int, gc_allocator<int> > sizes;

    int sent;

    Timer send_timer;
    send_timer.start();

    while (index < message.length()) {

        if (index + chunk > message.length()) {
            chunk = message.length() - index;
        }
        const char* data = message.data() + index;

        starts.push_back(Utils::get_current_time_microseconds_64());
        sent = api->custom_send(client, data, chunk, 0);
        ends.push_back(Utils::get_current_time_microseconds_64());
        sizes.push_back(sent);



        index += sent;
    }
    send_timer.stop();
    api->custom_close(client);

    while (!starts.empty()) {
        cout << "send " << starts.front() << " " << ends.front() << " " << sizes.front() << endl;
        starts.pop_front();
        ends.pop_front();
        sizes.pop_front();
    }

    cout << "Duration (us) to send " << index << " bytes to " << to_connect.to_s() << ": " << send_timer.get_duration_microseconds() << endl;


    sleep(1);
}
