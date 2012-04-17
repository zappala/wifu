#include "../headers/TCPTahoeSendReceivePassiveToActiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks1000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks65535) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks87380) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks1000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestPassiveToActiveBigChunks10000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APDelayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, APDelayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}