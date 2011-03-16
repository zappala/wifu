#include "MockNetworkInterface.h"
#include "exceptions/IOError.h"

MockNetworkInterface& MockNetworkInterface::instance() {
    static MockNetworkInterface instance_;
    return instance_;
}

MockNetworkInterface::~MockNetworkInterface() {

}

void MockNetworkInterface::start() {
}

void MockNetworkInterface::register_protocol(int protocol, PacketFactory* pf) {
}

void MockNetworkInterface::network_receive(WiFuPacket* p) {
}

void MockNetworkInterface::network_send(Event* e) {
    NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
    WiFuPacket* p = event->get_packet();
    TCPPacket* tcp_packet = (TCPPacket*) p;

    int delay = get_delay(tcp_packet);

    // drop the packet
//    cout << "MockNetowrkInterface::network_send(), Delay: " << delay << endl;
    if (delay == -1) {
//        cout << "MockNetowrkInterface::network_send(), Dropping packet" << endl;
        return;
    }
    
    usleep(delay);

    AddressPort* local = p->get_dest_address_port();
    AddressPort* remote = p->get_source_address_port();

    Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

    if (!s) {
        s = SocketCollection::instance().get_by_local_ap(local);
    }

    if (!s) {
        // No bound local socket
        return;
    }

    cout << "Received Packet: " << endl;
    

    Event* response = new NetworkReceivePacketEvent(s, p);
    Dispatcher::instance().enqueue(response);
}

MockNetworkInterface::MockNetworkInterface() : INetworkInterface() {
    read_config_file();

    srand(time(NULL));
    percent_ = -1;
}

int MockNetworkInterface::get_delay(TCPPacket* p) {
    int delay = 0;

    if(percent_ > 0) {
        int random = rand() % 100 + 1;
        if(random <= percent_) {
            delay = -1;
        }
    }
    else if (!control_nums_to_delay_.empty()) {
        pair<pair<int, int>, int> numbers = control_nums_to_delay_.front();
        int seq = numbers.first.first;
        int ack = numbers.first.second;

        if(seq == -1 && ack == -1) {
            percent_ = numbers.second;
        }

        if (p->get_tcp_sequence_number() == seq && p->get_tcp_ack_number() == ack) {
            // erase front
            control_nums_to_delay_.erase(control_nums_to_delay_.begin());
            delay = numbers.second;
        }
    }

    return delay;
}

void MockNetworkInterface::read_config_file() {
    string mockfile = "mockfile";
    if (!OptionParser::instance().present(mockfile)) {
        throw IllegalStateException();
    }
    string file = OptionParser::instance().argument(mockfile);

    try {

        vector<string> lines = Utils::read_file(file);
        string delim = " ";

        for (int i = 0; i < lines.size(); ++i) {
            string current = lines[i];

            vector<string> tokens = Utils::tokenize(current, delim);
            assert(tokens.size() == 3);

            int ack = atoi(tokens[0].c_str());
            int seq = atoi(tokens[1].c_str());
            int delay = atoi(tokens[2].c_str());

            pair<int, int> packet = make_pair(ack, seq);
            pair<pair<int, int>, int> p = make_pair(packet, delay);
            control_nums_to_delay_.push_back(p);

        }

    }
    catch(IOError e) {
        // TODO: make this better somehow.  (Print to stderr?)
//        cout << "Error in parsing/reading file" << endl;
    }
}
