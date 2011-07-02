#include "../headers/TCPTahoeSendReceiveActiveToPassive.h"

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive1) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(1));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive5) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(5));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive10) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(10));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive20) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(20));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive100) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(100));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive1000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive2000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(2000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive5000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(5000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive50000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveToPassive100000) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(BackEndMockTestDrop32, delayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(500));
}

TEST_F(BackEndMockTestDrop22Drop22, delayedACKSendReceiveTestActiveToPassiveDrop32) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(500));
}

TEST_F(BackEndMockTestDropRandom1Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom5Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom20Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom30Percent, delayedACKSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(TCP_DELAYEDACK, random_string(50000));
}