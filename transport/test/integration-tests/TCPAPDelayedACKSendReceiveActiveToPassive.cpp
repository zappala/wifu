#include "../headers/TCPTahoeSendReceiveActiveToPassive.h"

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive1) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive5) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive10) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive20) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive100) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive1000) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive2000) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive5000) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive50000) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassive100000) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDrop32, APDelayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(500));
}

TEST_F(WiFuTransportMockTestDrop22Drop22, APDelayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(500));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, APDelayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, APDelayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APDelayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_AP_DELAYEDACK, random_string(50000));
}