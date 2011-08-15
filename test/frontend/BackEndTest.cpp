#include "../headers/BackEndTest.h"

void BackEndTest::SetUp() {
    start_backend();
}

void BackEndTest::TearDown() {
    kill_backend();
}

void BackEndTest::start_backend() {
    gcstring commandToExecute = get_command();
    int value = system(commandToExecute.c_str()); 
    if (value < 0)
        FAIL() << "Error starting wifu-end";
    // We have to sleep so we can ensure the back end is up and running
    sleep(1);
}

void BackEndTest::kill_backend() {
    int value = system("killall wifu-end");
    if (value < 0) {
        FAIL() << "Error killing wifu-end";
    }
    sleep(1);
}

gcstring BackEndTest::get_command() {
    return "./wifu-end --logger_threshold 1000000 --logger_timeout 1000000.0 --network standard";
}

gcstring BackEndTest::getOutputFromCommand(char* cmd) {
    //_popen on Windows
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        ADD_FAILURE() << "Error opening pipe to find wifu-end";
    char buffer[128];
    gcstring result = "";
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    //_pclose on Windows
    pclose(pipe);
    return result;
}

gcstring BackEndMockTest::get_command() {
    gcstring cmd = "./wifu-end --logger_threshold 1 --logger_timeout 1.0 --network mock --passive_port 1000 --mockfile ";
    cmd.append(get_mock_file());
    return cmd;
}

gcstring BackEndMockTestDropNone::get_mock_file() {
    return "drop_none.conf";
}

gcstring BackEndMockTestDrop10::get_mock_file() {
    return "drop_1_0.conf";
}

gcstring BackEndMockTestDrop12::get_mock_file() {
    return "drop_1_2.conf";
}

gcstring BackEndMockTestDrop12Delay12::get_mock_file() {
    return "drop_1_2_delay_1_2.conf";
}

gcstring BackEndMockTestDrop12Delay10::get_mock_file() {
    return "drop_1_2_delay_1_0.conf";
}

gcstring BackEndMockTestDrop22::get_mock_file() {
    return "drop_2_2.conf";
}

gcstring BackEndMockTestDrop22Drop32::get_mock_file() {
    return "drop_2_2_drop_3_2.conf";
}

gcstring BackEndMockTestDrop22Drop22::get_mock_file() {
    return "drop_2_2_drop_2_2.conf";
}

gcstring BackEndMockTestDrop22Delay32::get_mock_file() {
    return "drop_2_2_delay_3_2.conf";
}

gcstring BackEndMockTestDrop33::get_mock_file() {
    return "drop_3_3.conf";
}

gcstring BackEndMockTestDrop34::get_mock_file() {
    return "drop_3_4.conf";
}
gcstring BackEndMockTestDrop23::get_mock_file() {
    return "drop_2_3.conf";
}

gcstring BackEndMockTestDrop24::get_mock_file() {
    return "drop_2_4.conf";
}

gcstring BackEndMockTestDropSecond24::get_mock_file() {
    return "drop_second_2_4.conf";
}

gcstring BackEndMockTestDrop24Drop24::get_mock_file() {
    return "drop_2_4_drop_2_4.conf";
}

gcstring BackEndMockTestDrop32::get_mock_file() {
    return "drop_3_2.conf";
}

gcstring BackEndMockTestDrop43::get_mock_file() {
    return "drop_4_3.conf";
}

gcstring BackEndMockTestDrop44::get_mock_file() {
    return "drop_4_4.conf";
}

gcstring BackEndMockTestDrop35::get_mock_file() {
    return "drop_3_5.conf";
}

gcstring BackEndMockTestDrop45::get_mock_file() {
    return "drop_4_5.conf";
}

gcstring BackEndMockTestDrop46::get_mock_file() {
    return "drop_4_6.conf";
}

gcstring BackEndMockTestDrop54::get_mock_file() {
    return "drop_5_4.conf";
}

gcstring BackEndMockTestDrop55::get_mock_file() {
    return "drop_5_5.conf";
}

gcstring BackEndMockTestDrop44Drop54::get_mock_file() {
    return "drop_4_4_drop_5_4.conf";
}

gcstring BackEndMockTestDrop35Drop45::get_mock_file() {
    return "drop_3_5_drop_4_5.conf";
}

gcstring BackEndMockTestDropRandom1Percent::get_mock_file() {
    return "drop_random_1_percent.conf";
}

gcstring BackEndMockTestDropRandom5Percent::get_mock_file() {
    return "drop_random_5_percent.conf";
}

gcstring BackEndMockTestDropRandom10Percent::get_mock_file() {
    return "drop_random_10_percent.conf";
}

gcstring BackEndMockTestDropRandom20Percent::get_mock_file() {
    return "drop_random_20_percent.conf";
}

gcstring BackEndMockTestDropRandom30Percent::get_mock_file() {
    return "drop_random_30_percent.conf";
}

gcstring BackEndMockTestDropRandom40Percent::get_mock_file() {
    return "drop_random_40_percent.conf";
}

gcstring BackEndMockTestDropRandom50Percent::get_mock_file() {
    return "drop_random_50_percent.conf";
}

gcstring BackEndMockTestDropRandom60Percent::get_mock_file() {
    return "drop_random_60_percent.conf";
}

gcstring BackEndMockTestDropRandom70Percent::get_mock_file() {
    return "drop_random_70_percent.conf";
}

gcstring BackEndMockTestDropRandom80Percent::get_mock_file() {
    return "drop_random_80_percent.conf";
}

gcstring BackEndMockTestDropRandom90Percent::get_mock_file() {
    return "drop_random_90_percent.conf";
}


