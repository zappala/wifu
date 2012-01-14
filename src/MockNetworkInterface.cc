#include "MockNetworkInterface.h"

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

void MockNetworkInterface::imodule_network_receive(WiFuPacket* p) {
}

void MockNetworkInterface::imodule_network_send(Event* e) {
    //cout << "MockNetworkInterface::network_send()" << endl;

    NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
    WiFuPacket* p = event->get_packet();
    int delay = 0;

    p->calculate_and_set_ip_checksum();


    TCPPacket* tcp_packet = (TCPPacket*) p;
    tcp_packet->pack();

    delay = get_delay(tcp_packet);

    assert(p);

    logger.log(p);

    if (delay == -1) {
        return;
    }

    if (delay > 0) {
        TimeoutEvent* timer = new TimeoutEvent(fake_socket_, 0, delay * 1000);
        delayed_[timer] = p;
        Dispatcher::instance().enqueue(timer);
        return;
    }


    receive(p);
}

void MockNetworkInterface::receive(WiFuPacket* p) {

    if (!p->is_valid_ip_checksum()) {
        return;
    }


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

    //    cout << "Socket " << s << " received packet: " << endl;
    //    cout << "MockNetworkInterface::network_send() (mocking a receive)" << endl;
    logger.log(p);
    //    cout << p->to_s_format() << endl;
    //    cout << p->to_s() << endl << endl;


    Event* response = new NetworkReceivePacketEvent(s, p);
    Dispatcher::instance().enqueue(response);
}

void MockNetworkInterface::imodule_timer_fired(Event* e) {
    TimerFiredEvent* event = (TimerFiredEvent*) e;

    map<TimeoutEvent*, WiFuPacket*, std::less<TimeoutEvent*>, gc_allocator<std::pair<TimeoutEvent*, WiFuPacket*> > >::iterator itr = delayed_.find(event->get_timeout_event());
    if (itr != delayed_.end()) {
        //        cout << "MockNetworkInterface::timer_fired()" << endl;
        receive(itr->second);
        delayed_.erase(itr);
    }
}

MockNetworkInterface::MockNetworkInterface() : INetworkInterface(), udp_seq_(0) {
    read_config_file();

    srand(time(NULL));
    percent_ = -1;

    fake_socket_ = new Socket(0, 0, 0);
}

int MockNetworkInterface::get_delay(TCPPacket* p) {
    int delay = 0;

    if (percent_ > 0) {
        int random = rand() % 100 + 1;
        if (random <= percent_) {
            delay = -1;
        }
    } else if (!control_nums_to_delay_.empty()) {
        pair<pair<int, int>, int> numbers = control_nums_to_delay_.front();
        int seq = numbers.first.first;
        int ack = numbers.first.second;

        if (seq == -1 && ack == -1) {
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
    gcstring mockfile = "mockfile";
    if (!OptionParser::instance().present(mockfile)) {
        throw IllegalStateException();
    }
    gcstring file = OptionParser::instance().argument(mockfile);

    try {

        vector<gcstring, gc_allocator<gcstring> > lines = Utils::read_file(file);
        gcstring delim = " ";

        for (int i = 0; i < lines.size(); ++i) {
            gcstring current = lines[i];

            vector<gcstring, gc_allocator<gcstring> > tokens = Utils::tokenize(current, delim);
            assert(tokens.size() == 3);

            int ack = atoi(tokens[0].c_str());
            int seq = atoi(tokens[1].c_str());
            int delay = atoi(tokens[2].c_str());

            pair<int, int> packet = make_pair(ack, seq);
            pair<pair<int, int>, int> p = make_pair(packet, delay);
            control_nums_to_delay_.push_back(p);

        }

    } catch (IOError e) {
        // TODO: make this better somehow.  (Print to stderr?)
        cout << "Error in parsing/reading file" << endl;
        assert(false);
    }
}
