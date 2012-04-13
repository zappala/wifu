#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, APSendReceiveTestActiveToPassiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, APSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP, random_string(100000));
}