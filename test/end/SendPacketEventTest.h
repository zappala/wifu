/*
 * SendPacketEventTest.h
 *
 *  Created on: Dec 17, 2010
 *      Author: erickson
 */

#ifndef SENDPACKETEVENTTEST_H_
#define SENDPACKETEVENTTEST_H_

#include "UnitTest++.h"
#include "../headers/events/SendPacketEvent.h"
#include "../headers/IModule.h"

using namespace std;

namespace {
	SUITE(SendPacketEvent) {

		class IModuleDummyImplementation : public IModule {
		public:
			IModuleDummyImplementation() {
				udp_sent = false;
			}

			void udp_send(Event* e) {
				udp_sent = true;
			}

			bool udp_sent;
		};

		TEST(udp_send) {
			IModuleDummyImplementation dummyImodule;
			CHECK(dummyImodule.udp_sent == false);


			int data_length = 10;
			unsigned char data[data_length];
			string source_address("127.0.0.1");
			string destination_address("127.0.0.1");
			struct wifu_end_header header;
			header.packet_length = data_length + sizeof(struct wifu_end_header);
			Packet packet(&header, data, 10, source_address, destination_address);


			SendPacketEvent sendPacketEvent(100, &packet);
			sendPacketEvent.execute(&dummyImodule);

			CHECK(dummyImodule.udp_sent == true);
		}
	}
}

#endif /* SENDPACKETEVENTTEST_H_ */
