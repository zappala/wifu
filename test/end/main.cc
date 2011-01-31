/*
 * main.cc
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "UnitTest++.h"

#include "UDPSocketTest.h"
#include "UDPInterfaceTest.h"
#include "AddressPortTest.h"
#include "SemaphoreTest.h"
#include "BinarySemaphoreTest.h"
#include "IDGeneratorTest.h"
#include "QueueTest.h"

#include "LocalSocketFullDuplexTest.h"

#include "QueryStringParserTest.h"
#include "SocketDataMapTest.h"

#include "CanceledEventsTest.h"
#include "PacketReceivedEventTest.h"
#include "SendPacketEventTest.h"
#include "TimeoutEventTest.h"
#include "CancelTimerEventTest.h"
#include "TimerFiredEventTest.h"

#include "DispatcherTest.h"
#include "EventSetTest.h"
#include "LocalSocketReceiverTest.h"
#include "LocalSocketSenderTest.h"
#include "MainSemaphoreTest.h"
#include "RandomNumberSetTest.h"
#include "OptionParserTest.h"
#include "SocketDataTest.h"
#include "PortManagerTest.h"
#include "PriorityQueueTest.h"
#include "QueueProcessor.h"
#include "SocketManagerTest.h"
#include "TimeoutEventManagerTest.h"
#include "TimeoutEventManagerSemaphoreTest.h"
#include "TimeoutHelperTest.h"
#include "UtilsTest.h"
#include "WifuEndBackEndLibrary.h"
#include "SocketCollectionTest.h"
#include "HashSetTest.h"

#include "AlreadyBoundToAddressPortVisitorTest.h"
#include "TempTest.h"
#include "SourceGetterTest.h"

#include "packet/IPPacketTest.h"
#include "packet/WiFuPacketTest.h"
#include "packet/TCPPacketTest.h"

#include "RawSocketsTest.h"
#include "NetworkInterfaceTest.h"

using namespace UnitTest;

int main(int argc, char** argv) {
    std::cout << "Running end tests" << std::endl;
    return UnitTest::RunAllTests();
}
