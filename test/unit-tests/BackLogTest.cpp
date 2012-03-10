#include "BackLog.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "packet/WiFuPacket.h"
#include <iostream>
#include "gtest/gtest.h"

using namespace std;

namespace {

    TEST(BackLog, All) {
        int capacity = 2;
        BackLog bl(capacity);

        ConnectionEstablishedEvent e1(new Socket(0,0,0), new Socket(0,0,0));
        ConnectionEstablishedEvent e2(new Socket(0,0,0), new Socket(0,0,0));
        ASSERT_TRUE(bl.empty());
        ASSERT_EQ(0, bl.size());
        ASSERT_EQ(capacity, bl.capacity());

        ASSERT_TRUE(bl.push(&e1));
        ASSERT_FALSE(bl.empty());
        ASSERT_TRUE(bl.push(&e2));

        ASSERT_FALSE(bl.push(&e2));
        
        ASSERT_EQ(bl.pop(), &e1);
        ASSERT_FALSE(bl.empty());
        ASSERT_EQ(1, bl.size());
        ASSERT_EQ(capacity, bl.capacity());

        ASSERT_TRUE(bl.push(&e1));
        ASSERT_EQ(capacity, bl.capacity());
        ASSERT_EQ(capacity, bl.size());

        ASSERT_EQ(bl.pop(), &e2);
        ASSERT_EQ(bl.pop(), &e1);
        ASSERT_TRUE(bl.empty());
    }

}
