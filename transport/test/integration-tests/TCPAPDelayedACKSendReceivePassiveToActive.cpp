#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, APDelayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, APDelayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APDelayedACKSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP_DELAYEDACK, random_string(50000));
}
