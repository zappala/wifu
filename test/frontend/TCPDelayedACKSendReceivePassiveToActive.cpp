#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(1));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(5));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(10));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(20));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(100));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(2000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(5000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom1Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom5Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom10Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom20Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom30Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}
