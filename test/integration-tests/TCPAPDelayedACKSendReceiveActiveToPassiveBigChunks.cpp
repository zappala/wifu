#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(1000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(65535));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(87380));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(1000000));
}

TEST_F(WiFuTransportMockTestDropNone, APDelayedACKSendReceiveTestActiveToPassiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(10000000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestActiveToPassiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(10000));
}

TEST_F(WiFuTransportMockTestDropRandom10Percent, APDelayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportMockTestDropRandom20Percent, APDelayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}

TEST_F(WiFuTransportIntegrationTest, APDelayedACKSendReceiveTestActiveToPassiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_AP_DELAYEDACK, random_string(100000));
}