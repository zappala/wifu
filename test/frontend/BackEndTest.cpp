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

string BackEndMockTest::get_command() {
    string cmd = "./wifu-end --network mock --passive_port 1000 --mockfile ";
    cmd.append(get_mock_file());
    return cmd;
}

string BackEndMockTestDropNone::get_mock_file() {
    return "drop_none.conf";
}

string BackEndMockTestDrop10::get_mock_file() {
    return "drop_1_0.conf";
}

string BackEndMockTestDrop12::get_mock_file() {
    return "drop_1_2.conf";
}

string BackEndMockTestDrop12Delay12::get_mock_file() {
    return "drop_1_2_delay_1_2.conf";
}

string BackEndMockTestDrop12Delay10::get_mock_file() {
    return "drop_1_2_delay_1_0.conf";
}

string BackEndMockTestDrop22::get_mock_file() {
    return "drop_2_2.conf";
}

string BackEndMockTestDrop22Drop32::get_mock_file() {
    return "drop_2_2_drop_3_2.conf";
}

string BackEndMockTestDrop22Delay32::get_mock_file() {
    return "drop_2_2_delay_3_2.conf";
}

string BackEndMockTestDrop33::get_mock_file() {
    return "drop_3_3.conf";
}

string BackEndMockTestDrop34::get_mock_file() {
    return "drop_3_4.conf";
}
string BackEndMockTestDrop23::get_mock_file() {
    return "drop_2_3.conf";
}

string BackEndMockTestDrop24::get_mock_file() {
    return "drop_2_4.conf";
}

string BackEndMockTestDrop32::get_mock_file() {
    return "drop_3_2.conf";
}

string BackEndMockTestDrop43::get_mock_file() {
    return "drop_4_3.conf";
}

string BackEndMockTestDrop44::get_mock_file() {
    return "drop_4_4.conf";
}

string BackEndMockTestDrop35::get_mock_file() {
    return "drop_3_5.conf";
}

string BackEndMockTestDrop45::get_mock_file() {
    return "drop_4_5.conf";
}

string BackEndMockTestDrop46::get_mock_file() {
    return "drop_4_6.conf";
}

string BackEndMockTestDrop54::get_mock_file() {
    return "drop_5_4.conf";
}

string BackEndMockTestDrop55::get_mock_file() {
    return "drop_5_5.conf";
}

string BackEndMockTestDrop44Drop54::get_mock_file() {
    return "drop_4_4_drop_5_4.conf";
}

string BackEndMockTestDrop35Drop45::get_mock_file() {
    return "drop_3_5_drop_4_5.conf";
}

string BackEndMockTestDropRandom10Percent::get_mock_file() {
    return "drop_random_10_percent.conf";
}

string BackEndMockTestDropRandom20Percent::get_mock_file() {
    return "drop_random_20_percent.conf";
}

string BackEndMockTestDropRandom30Percent::get_mock_file() {
    return "drop_random_30_percent.conf";
}

string BackEndMockTestDropRandom40Percent::get_mock_file() {
    return "drop_random_40_percent.conf";
}

string BackEndMockTestDropRandom50Percent::get_mock_file() {
    return "drop_random_50_percent.conf";
}

string BackEndMockTestDropRandom60Percent::get_mock_file() {
    return "drop_random_60_percent.conf";
}

string BackEndMockTestDropRandom70Percent::get_mock_file() {
    return "drop_random_70_percent.conf";
}

string BackEndMockTestDropRandom80Percent::get_mock_file() {
    return "drop_random_80_percent.conf";
}

string BackEndMockTestDropRandom90Percent::get_mock_file() {
    return "drop_random_90_percent.conf";
}


