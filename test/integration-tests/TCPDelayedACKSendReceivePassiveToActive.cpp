#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom30Percent, delayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_DELAYEDACK, random_string(50000));
}
