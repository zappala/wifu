/* 
 * File:   BackEndTest.h
 * Author: rbuck
 *
 * Created on March 14, 2011, 11:27 AM
 */

#ifndef _BACKENDTEST_H
#define	_BACKENDTEST_H

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

class BackEndTest : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
    virtual void start_backend();
    virtual void kill_backend();
    virtual gcstring get_command();

private:
    gcstring getOutputFromCommand(char* cmd);
};

class BackEndMockTest : public BackEndTest {
public:

    virtual gcstring get_command();
    virtual gcstring get_mock_file() = 0;
};

// No drops

class BackEndMockTestDropNone : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop10 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop12 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop12Delay12 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop12Delay10 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop22 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop22Drop32 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop22Drop22 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop22Delay32 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

//Drops the 2 3 packet twice

class BackEndMockTestDrop23 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop24 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropSecond24 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop24Drop24 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop32 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop33 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop34 : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDrop43 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop44 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop35 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop45 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop46 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop54 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop55 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop44Drop54 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDrop35Drop45 : public BackEndMockTest {
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom1Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom5Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom10Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom20Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom30Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom40Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom50Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom60Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom70Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom80Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

class BackEndMockTestDropRandom90Percent : public BackEndMockTest {
public:
    gcstring get_mock_file();
};

#endif	/* _BACKENDTEST_H */

