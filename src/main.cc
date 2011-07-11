/* 
 * File:   main.cc
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:47 PM
 */

#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdexcept>
#include <string.h>
#include <assert.h>
#include <signal.h>

//data structures
#include "Queue.h"
#include "PriorityQueue.h"

//interfaces
#include "NetworkInterface.h"
#include "NetworkInterfaceFactory.h"
#include "StandardNetworkInterfaceCreator.h"
#include "MockNetworkInterfaceCreator.h"
#include "StandardPortManagerCreator.h"
#include "MockPortManagerCreator.h"

//events
#include "TimeoutEventManager.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotFullEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"

//protocols
#include "ProtocolManager.h"
#include "protocol/SimpleTCP.h"
#include "protocol/TCPTahoe.h"
#include "protocol/TCPATP.h"
#include "protocol/SimpleUDP.h"
#include "protocol/TCPTahoe_DelayedACKs.h"
#include "protocol/TCP_FeW.h"
//#include "protocol/TCPAP.h"

//packet types
#include "TCPPacketFactory.h"
#include "WiFuPacketFactory.h"
#include "ATPPacketFactory.h"
#include "UDPPacketFactory.h"

//other
#include "defines.h"
#include "OptionParser.h"
#include "Logger.h"
#include "MainSemaphore.h"
#include "Dispatcher.h"
#include "Socket.h"
#include "WifuEndBackEndLibrary.h"
#include "PortManagerFactory.h"


using namespace std;

#define dispatcher Dispatcher::instance()
#define optionparser OptionParser::instance()

void main_signal_manager(int sig) {
    switch (sig) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            MainSemaphore::instance().post();
            break;
        case SIGSEGV:
            signal(SIGSEGV, SIG_DFL);
            cout << "wifu-end Segmentation Fault" << endl;
            abort();
            break;
        default:
            break;
    }
}

void register_signals() {
    signal(SIGINT, main_signal_manager);
    signal(SIGQUIT, main_signal_manager);
    signal(SIGTERM, main_signal_manager);
    signal(SIGSEGV, main_signal_manager);
}

void register_simple_tcp() {
    ProtocolManager::instance().support(SIMPLE_TCP);
    NetworkInterfaceFactory::instance().create().register_protocol(SIMPLE_TCP, new TCPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(BindEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ListenEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ConnectEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(AcceptEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ResendPacketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SendEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(CloseEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &SimpleTCP::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &SimpleTCP::instance());
}

void register_tcp_tahoe() {
    ProtocolManager::instance().support(TCP_TAHOE);
    NetworkInterfaceFactory::instance().create().register_protocol(TCP_TAHOE, new TCPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(BindEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ListenEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ConnectEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(AcceptEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ResendPacketEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(SendEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(CloseEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &TCPTahoe::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &TCPTahoe::instance());
}

void register_tcp_atp() {

    ProtocolManager::instance().support(TCP_ATP);
    NetworkInterfaceFactory::instance().create().register_protocol(TCP_ATP, new ATPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(BindEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ListenEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ConnectEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(AcceptEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ResendPacketEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(SendEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(CloseEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &TCPATP::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &TCPATP::instance());
}

void register_udp() {
    ProtocolManager::instance().support(UDP);
    NetworkInterfaceFactory::instance().create().register_protocol(UDP, new UDPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(BindEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(ListenEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(ConnectEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(AcceptEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(SendEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(CloseEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &SimpleUDP::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &SimpleUDP::instance());
}

void register_delayed_ack() {
    ProtocolManager::instance().support(TCP_DELAYEDACK);
    NetworkInterfaceFactory::instance().create().register_protocol(TCP_DELAYEDACK, new TCPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(BindEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ListenEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ConnectEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(AcceptEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ResendPacketEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(SendEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(CloseEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &TCPDelayedACK::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &TCPDelayedACK::instance());
}

void register_few() {
    ProtocolManager::instance().support(TCP_FEW);
    NetworkInterfaceFactory::instance().create().register_protocol(TCP_FEW, new TCPPacketFactory());

    dispatcher.map_event(type_name(SocketEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(BindEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ListenEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ConnectEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(AcceptEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(SendPacketEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ResendPacketEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ReceiveEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(SendEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(SendBufferNotFullEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(CloseEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(DeleteSocketEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(SetSocketOptionEvent), &TCP_FeW::instance());
    dispatcher.map_event(type_name(GetSocketOptionEvent), &TCP_FeW::instance());
}

void register_ap() {
    ProtocolManager::instance().support(TCP_AP);
    NetworkInterfaceFactory::instance().create().register_protocol(TCP_AP, new TCPPacketFactory());

    //    dispatcher.map_event(type_name(SocketEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(BindEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ListenEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ConnectEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(AcceptEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ConnectionEstablishedEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ConnectionInitiatedEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(SendPacketEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(NetworkReceivePacketEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(TimerFiredEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ResendPacketEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ReceiveEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(SendEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(SendBufferNotEmptyEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(SendBufferNotFullEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ReceiveBufferNotEmptyEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(ReceiveBufferNotFullEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(CloseEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(DeleteSocketEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(SetSocketOptionEvent), &TCPAP::instance());
    //    dispatcher.map_event(type_name(GetSocketOptionEvent), &TCPAP::instance());
}

void register_protocols() {
    // TODO: figure out a better way to register protocols via a config file
    register_simple_tcp();
    register_tcp_tahoe();
    register_tcp_atp();
    register_udp();
    register_delayed_ack();
    register_few();
    register_ap();
}

void setup_network_interface(gcstring& type) {
    if (type == "standard") {
        log_INFORMATIONAL("Using standard network interface");
        NetworkInterfaceFactory::instance().set_creator(new StandardNetworkInterfaceCreator());
        PortManagerFactory::instance().set_creator(new StandardPortManagerCreator());
    } else if (type == "mock") {
        log_INFORMATIONAL("Using mock network interface");
        NetworkInterfaceFactory::instance().set_creator(new MockNetworkInterfaceCreator());
        PortManagerFactory::instance().set_creator(new MockPortManagerCreator());
    }
}

int main(int argc, char** argv) {
    GC_INIT();



    //only needs to be on if logging to file
    //pantheios_be_file_setFilePath(PantheiosString("wifu-end.log"), PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_ALL);

    //log_DEBUG("main(", pantheios::args(argc, argv), ")");

    //TODO: Change second argument to 0 once we have a logger in place
    daemon(1, 1);

    gcstring network_type = "standard";
    gcstring network = "network";
    gcstring mockfile = "mockfile";
    gcstring passive_port = "passive_port";
    gcstring logger_threshold = "logger_threshold";
    gcstring logger_timeout = "logger_timeout";


    static struct option long_options[] = {
        {network.c_str(), required_argument, NULL, 0},
        {mockfile.c_str(), required_argument, NULL, 0},
        {passive_port.c_str(), required_argument, NULL, 0},
        {logger_threshold.c_str(), required_argument, NULL, 0},
        {logger_timeout.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    optionparser.parse(argc, argv, long_options);
    if (optionparser.present(network)) {
        network_type = optionparser.argument(network);
    }

    setup_network_interface(network_type);
    register_signals();

    // Start Dispatcher
    dispatcher.start_processing();

    // Start Back end
    WifuEndBackEndLibrary::instance();

    // Load Modules
    dispatcher.map_event(type_name(NetworkSendPacketEvent), &NetworkInterfaceFactory::instance().create());
    dispatcher.map_event(type_name(TimerFiredEvent), &NetworkInterfaceFactory::instance().create());
    dispatcher.map_event(type_name(TimeoutEvent), &TimeoutEventManager::instance());
    dispatcher.map_event(type_name(CancelTimerEvent), &TimeoutEventManager::instance());
    dispatcher.map_event(type_name(ResponseEvent), &WifuEndBackEndLibrary::instance());
    dispatcher.map_event(type_name(TimerFiredEvent), &PacketLogger::instance());

    // Packet Logger
    if (optionparser.present(logger_threshold)) {
        int value = atoi(optionparser.argument(logger_threshold).c_str());
        PacketLogger::instance().set_flush_threshold(value);
    }

    if (optionparser.present(logger_timeout)) {
        double value = atof(optionparser.argument(logger_timeout).c_str());
        PacketLogger::instance().set_flush_timeout(value);
    }
    PacketLogger::instance().reset();

    register_protocols();

    NetworkInterfaceFactory::instance().create().start();

    // Wait indefinitely
    MainSemaphore::instance().init(0);
    MainSemaphore::instance().wait();

    // This probably is not be needed but it's here in case it turns out to be
    // Closes the file by setting the path to NULL
    // pantheios_be_file_setFilePath(NULL, PANTHEIOS_BEID_ALL);

    return (EXIT_SUCCESS);
}
