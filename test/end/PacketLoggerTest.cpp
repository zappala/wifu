/*
 * PacketLoggerTest.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "packet/TCPPacket.h"
#include "defines.h"

/*
 * Only the following two need to be included.
 */
#include "PacketLogger.h"
#include "PacketLogReader.h"

using namespace std;

/*
 * Convenience only.
 */
#define logger PacketLogger::instance()

namespace {

    TEST(PacketLogger, test) {
        TCPPacket* packet = new TCPPacket(); //be sure to call 'new'
        packet->set_ip_protocol(SIMPLE_TCP); //very important
        unsigned char data1[] = "hello"; //whatever data, could be blank ("") too I suppose (which won't be needed when we update the init method to set this right off the bat for when you don't want any data in the packet)
        packet->set_data(data1, sizeof (data1)); //set_data has to be called so that ip_tot is updated

        /*
         * These should only be called in the RawSocket* files (which they are right now).
         * They are called here so that a fake network trace is written to the log.
         */
        logger.log(packet);
        logger.log(packet);

        NetworkTrace expected; //the sent and received packets that you expect from the daemon's perspective
        expected.add_packet(packet);
        expected.add_packet(packet);

        PacketLogReader log_reader(LOG_FILENAME);
        NetworkTrace* actual = log_reader.get_trace();

        /*
         * The order is important on the streaming.
         * The object that has "get_packet_trace" called on it assumes that it is the EXPECTED packet trace.
         * 'actual' is dereferenced so that the operator== is called correctly.
         */
        ASSERT_EQ(expected, *actual) << expected.get_packet_trace(*actual); //
    }
}
