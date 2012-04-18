#include "../headers/TCPTahoeSendReceivePassiveToActiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks1000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks65535) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks87380) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks1000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestPassiveToActiveBigChunks10000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, APSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_AP, random_string(100000));
}