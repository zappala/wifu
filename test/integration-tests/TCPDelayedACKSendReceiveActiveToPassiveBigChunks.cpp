#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, delayedACKSendReceiveTestActiveToPassiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, delayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, delayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}
