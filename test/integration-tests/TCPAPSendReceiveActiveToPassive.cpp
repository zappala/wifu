#include "../headers/TCPTahoeSendReceiveActiveToPassive.h"

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive1) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive5) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive10) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive20) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive100) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive1000) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive2000) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive5000) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive50000) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassive100000) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportMockTestDrop32, APSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(500));
}

TEST_F(WiFuTransportMockTestDrop22Drop22, APSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(500));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, APSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, APSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom30Percent, APSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP, random_string(50000));
}