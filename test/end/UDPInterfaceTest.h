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
        t.update_stop();
        s.post();
    }

    Packet* p;
    Timer t;
    Semaphore s;
};

namespace {

    SUITE(UDPInterface) {

        TEST(UDPInterfaceTest) {
            TempUDPModule temp;
            usleep(10000);

            AddressPort ap("127.0.0.1", WIFU_PORT);
            udp.start(ap);
            usleep(10000);

            dispatcher.map_event(type_name(UDPReceivePacketEvent), &temp);
            dispatcher.start_processing();

            AddressPort* source = new AddressPort("127.0.0.1", 5000);
            AddressPort* dest = new AddressPort("127.0.0.1", 5001);

            Socket* s = new Socket(0, 0, 0, dest, source);
            SocketCollection::instance().push(s);

            const char* data = "Hello";
            int length = 5;

            Packet* p = new Packet(source, dest, (unsigned char*) data, length);
            UDPSendPacketEvent* e = new UDPSendPacketEvent(s->get_socket(), p);
            udp.udp_send(e);
            temp.t.start();
            temp.s.wait();


            CHECK_EQUAL(data, (const char*) temp.p->get_data());
            CHECK_EQUAL(length, temp.p->data_length());
            CHECK(*dest == *(temp.p->get_destination()));
            CHECK(*source == *(temp.p->get_source()));
            cout << "Time (us) to send and receive one packet: " << temp.t.get_duration_microseconds() << endl;

        }
    }
}

#endif	/* _UDPINTERFACETEST_H */

