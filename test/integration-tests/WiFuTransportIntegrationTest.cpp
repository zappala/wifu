#include "../headers/WiFuTransportIntegrationTest.h"

void WiFuTransportIntegrationTest::SetUp() {
    start_backend();
}

void WiFuTransportIntegrationTest::TearDown() {
    kill_backend();
}

void WiFuTransportIntegrationTest::start_backend() {
    gcstring commandToExecute = get_command();
    int value = system(commandToExecute.c_str()); 
    if (value < 0)
        FAIL() << "Error starting wifu-transport";
    // We have to sleep so we can ensure the back end is up and running
    sleep(1);
}

void WiFuTransportIntegrationTest::kill_backend() {
    int value = system("killall wifu-transport");
    if (value < 0) {
        FAIL() << "Error killing wifu-transport";
    }
    sleep(1);
}

gcstring WiFuTransportIntegrationTest::get_command() {
    //TODO: turn on all supported protocols for now
    return "./wifu-transport --logger_threshold 1000000 --logger_timeout 1000000.0 --network standard --tahoe --ap --dack --udp --apdack ";
}

gcstring WiFuTransportIntegrationTest::getOutputFromCommand(char* cmd) {
    //_popen on Windows
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        ADD_FAILURE() << "Error opening pipe to find wifu-transport";
    char buffer[128];
    gcstring result = "";
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    //_pclose on Windows
    pclose(pipe);
    return result;
}

gcstring WiFuTransportIntegrationMockTest::get_command() {
    gcstring cmd = "./wifu-transport --logger_threshold 1 --logger_timeout 1.0 --network mock --passive_port 1000 --tahoe --ap --dack --udp --apdack --mockfile ";
    cmd.append(get_mock_file());
    return cmd;
}

gcstring WiFuTransportMockTestDropNone::get_mock_file() {
    return "drop_none.conf";
}

gcstring WiFuTransportMockTestDrop10::get_mock_file() {
    return "drop_1_0.conf";
}

gcstring WiFuTransportMockTestDrop12::get_mock_file() {
    return "drop_1_2.conf";
}

gcstring WiFuTransportMockTestDrop12Delay12::get_mock_file() {
    return "drop_1_2_delay_1_2.conf";
}

gcstring WiFuTransportMockTestDrop12Delay10::get_mock_file() {
    return "drop_1_2_delay_1_0.conf";
}

gcstring WiFuTransportMockTestDrop22::get_mock_file() {
    return "drop_2_2.conf";
}

gcstring WiFuTransportMockTestDrop22Drop32::get_mock_file() {
    return "drop_2_2_drop_3_2.conf";
}

gcstring WiFuTransportMockTestDrop22Drop22::get_mock_file() {
    return "drop_2_2_drop_2_2.conf";
}

gcstring WiFuTransportMockTestDrop22Delay32::get_mock_file() {
    return "drop_2_2_delay_3_2.conf";
}

gcstring WiFuTransportMockTestDrop33::get_mock_file() {
    return "drop_3_3.conf";
}

gcstring WiFuTransportMockTestDrop34::get_mock_file() {
    return "drop_3_4.conf";
}
gcstring WiFuTransportMockTestDrop23::get_mock_file() {
    return "drop_2_3.conf";
}

gcstring WiFuTransportMockTestDrop24::get_mock_file() {
    return "drop_2_4.conf";
}

gcstring WiFuTransportMockTestDropSecond24::get_mock_file() {
    return "drop_second_2_4.conf";
}

gcstring WiFuTransportMockTestDrop24Drop24::get_mock_file() {
    return "drop_2_4_drop_2_4.conf";
}

gcstring WiFuTransportMockTestDrop32::get_mock_file() {
    return "drop_3_2.conf";
}

gcstring WiFuTransportMockTestDrop43::get_mock_file() {
    return "drop_4_3.conf";
}

gcstring WiFuTransportMockTestDrop44::get_mock_file() {
    return "drop_4_4.conf";
}

gcstring WiFuTransportMockTestDrop35::get_mock_file() {
    return "drop_3_5.conf";
}

gcstring WiFuTransportMockTestDrop45::get_mock_file() {
    return "drop_4_5.conf";
}

gcstring WiFuTransportMockTestDrop46::get_mock_file() {
    return "drop_4_6.conf";
}

gcstring WiFuTransportMockTestDrop54::get_mock_file() {
    return "drop_5_4.conf";
}

gcstring WiFuTransportMockTestDrop55::get_mock_file() {
    return "drop_5_5.conf";
}

gcstring BackEndMockTestDrop44Drop54::get_mock_file() {
    return "drop_4_4_drop_5_4.conf";
}

gcstring WiFuTransportMockTestDrop35Drop45::get_mock_file() {
    return "drop_3_5_drop_4_5.conf";
}

gcstring WiFuTransportMockTestDropRandom1Percent::get_mock_file() {
    return "drop_random_1_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom5Percent::get_mock_file() {
    return "drop_random_5_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom10Percent::get_mock_file() {
    return "drop_random_10_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom20Percent::get_mock_file() {
    return "drop_random_20_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom30Percent::get_mock_file() {
    return "drop_random_30_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom40Percent::get_mock_file() {
    return "drop_random_40_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom50Percent::get_mock_file() {
    return "drop_random_50_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom60Percent::get_mock_file() {
    return "drop_random_60_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom70Percent::get_mock_file() {
    return "drop_random_70_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom80Percent::get_mock_file() {
    return "drop_random_80_percent.conf";
}

gcstring WiFuTransportMockTestDropRandom90Percent::get_mock_file() {
    return "drop_random_90_percent.conf";
}


