#include "../headers/TCPTahoeSendReceivePassiveToActiveBigChunks.h"

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks1000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(10000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks65535) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(65535));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks87380) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(87380));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(100000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks1000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(1000000));
}

TEST_F(BackEndMockTestDropNone, fewSendReceiveTestPassiveToActiveBigChunks10000000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(10000000));
}

TEST_F(BackEndMockTestDropRandom10Percent, fewSendReceiveTestPassiveToActiveBigChunks10000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(10000));
}

TEST_F(BackEndMockTestDropRandom10Percent, fewSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(100000));
}

TEST_F(BackEndMockTestDropRandom20Percent, fewSendReceiveTestPassiveToActiveBigChunks100000) {
    tcp_tahoe_passive_to_active_big_chunks(TCP_FEW, random_string(100000));
}