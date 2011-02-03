/* 
 * File:   NetworkInterfaceTest.h
 * Author: rbuck
 *
 * Created on January 31, 2011, 10:54 AM
 */

#ifndef _NETWORKINTERFACETEST_H
#define	_NETWORKINTERFACETEST_H

#include "gtest/gtest.h"
#include "NetworkInterface.h"
#include "WiFuPacketFactory.h"
#include "Module.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/NetworkSendPacketEvent.h"
#include "headers/RandomStringGenerator.h"
#include "Timer.h"

using namespace std;

#define network NetworkInterface::instance()
#define dispatcher Dispatcher::instance()

namespace {

    class FakeNetworkModule : public Module {
    public:

        FakeNetworkModule() : Module() {
            s.init(0);
        }

        void network_receive(Event* e) {
            NetworkReceivePacketEvent* event = (NetworkReceivePacketEvent*) e;
            p = event->get_packet();
            s.post();
        }

        WiFuPacket* p;
        Semaphore s;
    };

    WiFuPacket * make_packet(int protocol, string & data, AddressPort* source, AddressPort* dest) {
        WiFuPacket* p = new WiFuPacket();
        p->set_ip_destination_address_s(dest->get_address());
        p->set_destination_port(dest->get_port());
        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());
        p->set_ip_protocol(protocol);
        p->set_data((unsigned char*) data.c_str(), data.length());
        return p;
    }

    TEST(NetworkInterface, all) {

        int protocol = 101;
        int num_packets = 100;
        int packet_size = 1450;

        network.register_protocol(protocol, new WiFuPacketFactory());
        network.start();
        usleep(10000);

        FakeNetworkModule fnm;
        dispatcher.map_event(type_name(NetworkReceivePacketEvent), &fnm);
        dispatcher.map_event(type_name(NetworkSendPacketEvent), &network);
        dispatcher.start_processing();

        AddressPort* source = new AddressPort("127.0.0.1", 5000);
        AddressPort* dest = new AddressPort("127.0.0.1", 5001);

        Socket* s = new Socket(0, 0, 0, dest);
        SocketCollection::instance().clear();
        SocketCollection::instance().push(s);

        // only by local address/port
        int time = 0;
        for (int i = 0; i < num_packets; i++) {

            string data = random_string(packet_size);

            Timer t;
            t.start();
            WiFuPacket* p = make_packet(protocol, data, source, dest);
            NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
            dispatcher.enqueue(e);
            fnm.s.wait();
            t.stop();
            time += t.get_duration_microseconds();

            string expected = data.c_str();
            string actual = (const char*) fnm.p->get_data();
            ASSERT_EQ(expected, actual);
            ASSERT_EQ(packet_size, fnm.p->get_data_length_bytes());
            ASSERT_TRUE(*dest == *(fnm.p->get_dest_address_port()));
            ASSERT_TRUE(*source == *(fnm.p->get_source_address_port()));
        }
        cout << "Local AddressPort only, Time (us) to send and receive " << num_packets << " packets: " << time << endl;

        // by local AND remote address/port
        s->set_remote_address_port(source);

        time = 0;
        for (int i = 0; i < num_packets; i++) {

            string data = random_string(packet_size);

            Timer t;
            t.start();
            WiFuPacket* p = make_packet(protocol, data, source, dest);
            NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
            dispatcher.enqueue(e);
            fnm.s.wait();
            t.stop();
            time += t.get_duration_microseconds();

            string expected = data.c_str();
            string actual = (const char*) fnm.p->get_data();
            ASSERT_EQ(expected, actual);
            ASSERT_EQ(packet_size, fnm.p->get_data_length_bytes());
            ASSERT_TRUE(*dest == *(fnm.p->get_dest_address_port()));
            ASSERT_TRUE(*source == *(fnm.p->get_source_address_port()));
        }
        cout << "Local and Remote AddressPort, Time (us) to send and receive " << num_packets << " packets: " << time << endl;

    }
}


#endif	/* _NETWORKINTERFACETEST_H */
