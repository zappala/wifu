#include "../headers/TCPTahoeSendReceivePassiveToActive.h"

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive1) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(1));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive5) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(5));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive10) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(10));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive20) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(20));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive100) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(100));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive1000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive2000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(2000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive5000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(5000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActive50000) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom1Percent, APSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom5Percent, APSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(50000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APSendReceiveTestPassiveToActiveDropRandom) {
    tahoe_passive_to_active_send_receive_test(TCP_AP, random_string(50000));
}
