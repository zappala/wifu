/* 
 * File:   WiFuTransportIntegrationTest.h
 * Author: rbuck
 *
 * Created on March 14, 2011, 11:27 AM
 */

#ifndef _WiFuTransportIntegrationTest_H
#define	_WiFuTransportIntegrationTest_H

#include "gtest/gtest.h"

#include "Semaphore.h"
#include "AddressPort.h"

using namespace std;

struct var {
    Semaphore* sem_;
    Semaphore* flag_;
    Semaphore* done_;
    Semaphore* done_sending_;
    AddressPort* to_bind_;
    //int countdown_;
    //int rec_buf_;
    gcstring expected_string;
    int protocol_;
        pthread_barrier_t* barrier;

};

struct udpvar {
    Semaphore* sem_;
    Semaphore* done_;
    AddressPort* to_bind_;
    int countdown_;
    int rec_buf_;
    gcstring expected_string;
};

class WiFuTransportIntegrationTest : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
    virtual void start_backend();
    virtual void kill_backend();
    virtual gcstring get_command();

private:
    gcstring getOutputFromCommand(char* cmd);
};

class WiFuTransportIntegrationMockTest : public WiFuTransportIntegrationTest {
public:

    virtual gcstring get_command();
    virtual gcstring get_mock_file() = 0;
};

// No drops

class WiFuTransportMockTestDropNone : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop10 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop12 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop12Delay12 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop12Delay10 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop22 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop22Drop32 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop22Drop22 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop22Delay32 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

//Drops the 2 3 packet twice

class WiFuTransportMockTestDrop23 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop24 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropSecond24 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop24Drop24 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop32 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop33 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop34 : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop43 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop44 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop35 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop45 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop46 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop54 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop55 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop44Drop54 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDrop35Drop45 : public WiFuTransportIntegrationMockTest {
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom1Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom5Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom10Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom20Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom30Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom40Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom50Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom60Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom70Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom80Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

class WiFuTransportMockTestDropRandom90Percent : public WiFuTransportIntegrationMockTest {
public:
    gcstring get_mock_file();
};

#endif	/* _WiFuTransportIntegrationTest_H */

