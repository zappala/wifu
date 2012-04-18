#include "../headers/TCPTahoeSendReceiveActiveToPassive.h"

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive1) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive5) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive10) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive20) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive100) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive1000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive2000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive5000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive50000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassive100000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDrop32, delayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(500));
}

TEST_F(WiFuTransportMockTestDrop22Drop22, delayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(500));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}
