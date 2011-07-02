#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(1));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(5));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(10));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(20));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(100));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(2000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(5000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom1Percent, fewSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom5Percent, fewSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom10Percent, fewSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom20Percent, fewSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}

TEST_F(BackEndMockTestDropRandom30Percent, fewSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_FEW, random_string(50000));
}
