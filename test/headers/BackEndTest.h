/* 
 * File:   BackEndTest.h
 * Author: rbuck
 *
 * Created on March 14, 2011, 11:27 AM
 */

#ifndef _BACKENDTEST_H
#define	_BACKENDTEST_H

#include <string>

#include "gtest/gtest.h"

#include "Semaphore.h"
#include "AddressPort.h"

using namespace std;

struct var {
    Semaphore* sem_;
    AddressPort* to_bind_;
    string expected_string;
};

class BackEndTest : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
    virtual void start_backend();
    virtual void kill_backend();
    virtual string get_command();

private:
    string getOutputFromCommand(char* cmd);
};

class BackEndMockTest : public BackEndTest {
public:

    virtual string get_command();
    virtual string get_mock_file() = 0;
};

// No drops

class BackEndMockTestDropNone : public BackEndMockTest {
public:
    string get_mock_file();
};

//Drops the 3 3 packet twice

class BackEndMockTestDrop33 : public BackEndMockTest {
public:
    string get_mock_file();
};

//Drops the 2 3 packet twice

class BackEndMockTestDrop23 : public BackEndMockTest {
public:
    string get_mock_file();
};

class BackEndMockTestDrop24 : public BackEndMockTest {
public:
    string get_mock_file();
};

class BackEndMockTestDrop32 : public BackEndMockTest {
public:
    string get_mock_file();
};

#endif	/* _BACKENDTEST_H */

