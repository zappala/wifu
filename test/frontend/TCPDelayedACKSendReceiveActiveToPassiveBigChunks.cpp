#include "../headers/TCPTahoeSendReceiveActiveToPassiveBigChunks.h"

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks1000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(1000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks65535) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(65535));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks87380) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(87380));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks1000000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(1000000));
}

TEST_F(BackEndMockTestDropNone, delayedACKSendReceiveTestActiveBigChunks10000000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000000));
}

TEST_F(BackEndMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveBigChunks10000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(10000));
}

TEST_F(BackEndMockTestDropRandom10Percent, delayedACKSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}

TEST_F(BackEndMockTestDropRandom20Percent, delayedACKSendReceiveTestActiveBigChunks100000) {
    tahoe_active_to_passive_big_chunks(TCP_DELAYEDACK, random_string(100000));
}