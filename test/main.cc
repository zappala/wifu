/*
 * test.cc
 *
 *  Created on: Oct 29, 2010
 *      Author: erickson
 */


#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "headers/CheckMacros.h"


#include "UDPSocketTest.h"
#include "AddressPortTest.h"
#include "SemaphoreTest.h"
#include "BinarySemaphoreTest.h"
#include "IDGeneratorTest.h"
#include "IdentifiableTest.h"
#include "QueueTest.h"
#include "PacketTest.h"

#include "LocalSocketFullDuplexTest.h"
#include "QueryStringParserTest.h"
#include "SocketDataMapTest.h"

#include "CanceledEventsTest.h"
#include "ConnectEventTest.h"
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

#include "WifuEndAPITest.h"
#include "IntegrationTest.h"

using namespace UnitTest;

void change_dir() {
    int size = 1000;
    char buf[size];
    getcwd(buf, size);
    string path = buf;
    string bin = "bin";

    if (path.find(bin) == string::npos) {
        chdir(bin.c_str());
    }
}

void cleanup() {
    int value = system("rm LS*");
    if (value < 0) {
        perror("Error Removing LS Files");
    }

    value = system("rm WS");
    if (value < 0) {
        perror("Error Removing WS File");
    }

}

int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;

    change_dir();
    int result = UnitTest::RunAllTests();
    //    cleanup();

    return result;
}
