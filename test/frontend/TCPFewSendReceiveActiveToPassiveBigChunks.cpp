#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(10000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(65535));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(87380));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(100000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(1000000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestActiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(10000000));
}

TEST_F(BackEndMockTestDropRandom10Percent, fewSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(10000));
}

TEST_F(BackEndMockTestDropRandom10Percent, fewSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(100000));
}

TEST_F(BackEndMockTestDropRandom20Percent, fewSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_FEW, random_string(100000));
}