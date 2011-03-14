#include "../headers/BackEndTest.h"

void BackEndTest::SetUp() {
    start_backend();
}

void BackEndTest::TearDown() {
    kill_backend();
}

void BackEndTest::start_backend() {
    string commandToExecute = get_command();
    int value = system(commandToExecute.c_str());
    if (value < 0)
        FAIL() << "Error starting wifu-end";
    // We have to sleep so we can ensure the back end is up and running
    sleep(1);
}

void BackEndTest::kill_backend() {
    int value = system("killall wifu-end");
    if (value < 0)
        FAIL() << "Error killing wifu-end";
}

string BackEndTest::get_command() {
    return "./wifu-end --network standard";
}

string BackEndTest::getOutputFromCommand(char* cmd) {
    //_popen on Windows
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        ADD_FAILURE() << "Error opening pipe to find wifu-end";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    //_pclose on Windows
    pclose(pipe);
    return result;
}

// BackEndMockTest

string BackEndMockTest::get_command() {
    string cmd = "./wifu-end --network mock --mockfile ";
    cmd.append(get_mock_file());
    return cmd;
}

// BackEndMockTestDropNone

string BackEndMockTestDropNone::get_mock_file() {
    return "drop_none.conf";
}

//Drops the 3 3 packet twice

string BackEndMockTestDrop33::get_mock_file() {
    return "drop_3_3.conf";
}

//Drops the 2 3 packet twice

string BackEndMockTestDrop23::get_mock_file() {
    return "drop_2_3.conf";
}

// BackEndMockTestDrop24

string BackEndMockTestDrop24::get_mock_file() {
    return "drop_2_4.conf";
}

// BackEndMockTestDrop32

string BackEndMockTestDrop32::get_mock_file() {
    return "drop_3_2.conf";
}


