/* 
 * File:   UDPInterfaceTest.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 3:09 PM
 */

#ifndef _UDPINTERFACETEST_H
#define	_UDPINTERFACETEST_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "UnitTest++.h"
#include "../headers/UDPInterface.h"
#include "../headers/Module.h"
#include "../headers/AddressPort.h"
#include "../headers/Dispatcher.h"
#include "../headers/events/UDPReceivePacketEvent.h"
#include "../headers/events/UDPSendPacketEvent.h"
#include "../headers/Socket.h"
#include "../preliminary/Timer.h"
#include "../headers/Semaphore.h"
#include "../headers/Packet.h"

#define udp UDPInterface::instance()
#define dispatcher Dispatcher::instance()

using namespace std;

class TempUDPModule : public Module {
public:

    TempUDPModule() : Module() {
        s.init(0);
    }

    void udp_receive(Event* e) {
        UDPReceivePacketEvent* event = (UDPReceivePacketEvent*) e;
        p = event->get_packet();
        s.post();
    }

    Packet* p;
    Semaphore s;
};

namespace {

    SUITE(UDPInterface) {

        string get_data(int packet_size) {
            string data = "";
            for (int i = 0; i < packet_size; i++) {
                // Thanks to: http://www.geekstogo.com/forum/topic/94906-random-ints-and-chars-in-c/
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                srand(ts.tv_nsec);
                char c = (rand() % 26) + 'a';
                string s(1, c);
                data.append(s);
            }
            return data;
        }

        TEST(UDPInterfaceTest) {
            int num_packets = 100;
            int packet_size = 1450;
            TempUDPModule temp;
            usleep(10000);

            string address = "127.0.0.1";
            AddressPort ap(address, WIFU_PORT);
            udp.start(ap);
            usleep(10000);

            CHECK_EQUAL(address, udp.get_bound_ip_address());

            dispatcher.map_event(type_name(UDPReceivePacketEvent), &temp);
            dispatcher.map_event(type_name(UDPSendPacketEvent), &udp);
            dispatcher.start_processing();

            AddressPort* source = new AddressPort("127.0.0.1", 5000);
            AddressPort* dest = new AddressPort("127.0.0.1", 5001);

            Socket* s = new Socket(0, 0, 0, dest);
            SocketCollection::instance().push(s);

            // only by local address/port
            int time = 0;
            for (int i = 0; i < num_packets; i++) {

                string data = get_data(packet_size);

                Timer t;
                t.start();
                Packet* p = new Packet(source, dest, (unsigned char*) data.c_str(), data.size());
                UDPSendPacketEvent* e = new UDPSendPacketEvent(s, p);
                dispatcher.enqueue(e);
                temp.s.wait();
                t.stop();
                time += t.get_duration_microseconds();

                CHECK_EQUAL(data.c_str(), (const char*) temp.p->get_data());
                CHECK_EQUAL(packet_size, temp.p->data_length());
                CHECK(*dest == *(temp.p->get_destination()));
                CHECK(*source == *(temp.p->get_source()));
            }
            cout << "Local AddressPort only, Time (us) to send and receive " << num_packets << " packets: " << time << endl;

            // by local AND remote address/port
            s->set_remote_address_port(source);

            time = 0;
            for (int i = 0; i < num_packets; i++) {

                string data = get_data(packet_size);

                Timer t;
                t.start();
                Packet* p = new Packet(source, dest, (unsigned char*) data.c_str(), data.size());
                UDPSendPacketEvent* e = new UDPSendPacketEvent(s, p);
                udp.udp_send(e);
                temp.s.wait();
                t.stop();
                time += t.get_duration_microseconds();

                CHECK_EQUAL(data.c_str(), (const char*) temp.p->get_data());
                CHECK_EQUAL(packet_size, temp.p->data_length());
                CHECK(*dest == *(temp.p->get_destination()));
                CHECK(*source == *(temp.p->get_source()));
            }
            cout << "Local and Remote AddressPort, Time (us) to send and receive " << num_packets << " packets: " << time << endl;
        }
    }
}

#endif	/* _UDPINTERFACETEST_H */

