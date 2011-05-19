/* 
 * File:   MockNetworkInterface.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 4:25 PM
 */

#ifndef _MOCKNETWORKINTERFACE_H
#define	_MOCKNETWORKINTERFACE_H


#include <map>

#include "INetworkInterface.h"
#include "events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "OptionParser.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/IOError.h"
#include "PacketLogger.h"

#define logger PacketLogger::instance()

class MockNetworkInterface : public INetworkInterface {
public:
    static MockNetworkInterface& instance();

    virtual ~MockNetworkInterface();

    void start();

    void register_protocol(int protocol, PacketFactory* pf);

    void imodule_network_receive(WiFuPacket* p);

    void imodule_network_send(Event* e);

    void imodule_timer_fired(Event* e);

private:
    MockNetworkInterface();


    /**
     * There are three possible outcomes of this function:
     * 1. The packet sequence and ack numbers match a line in the config file.  We either:
     *    a. Delay the packet (return > 0)
     *    b. Drop the packet (return -1)
     * 2. The config file indicates we are to randomly drop packets.  This is indicated by the config file reading -1 -1 N, where N is a number between 1 and 99 and indicates what percent of packets from now on we are to drop.  If the random number generator indicates that this packet should be dropped, this function returns -1.
     */
    int get_delay(TCPPacket* p);

    void read_config_file();

    void receive(WiFuPacket* p);

    // innermost pair is seq/ack number
    // outermost pair is pair of seq/ack number to delay
    // delay of -1 means drop
    vector< pair< pair<int, int>, int> > control_nums_to_delay_;

    int percent_;

    map<TimeoutEvent*, TCPPacket*> delayed_;

    Socket* fake_socket_;
};

#endif	/* _MOCKNETWORKINTERFACE_H */

