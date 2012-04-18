#include "../headers/TCPTahoeSendReceivePassiveToActiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks1000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks65535) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks87380) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks1000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestPassiveToActiveBigChunks10000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, delayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, delayedACKSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_DELAYEDACK, random_string(100000));
}
