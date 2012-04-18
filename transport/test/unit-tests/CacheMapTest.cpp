#include "gtest/gtest.h"
#include "exceptions/InvalidAddressException.h"
#include "CacheMap.h"

#include "Socket.h"
#include "Cache.h"

using namespace std;

#define map CacheMap::instance()

namespace {

    TEST(CacheMap, Constructor) {
        map;
    }

    TEST(CacheMap, putAndGet) {
        Socket* s = new Socket(0,0,0);
        Cache* expected = new Cache();
        map.put(s, expected);

        Cache* actual = map.get(s);
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, map.size());
        EXPECT_FALSE(map.empty());

        map.remove(s);
    }

    TEST(CacheMap, getNull) {
        Socket* bogus = new Socket(0,0,0);
        Cache* expected = NULL;
        Cache* actual = map.get(bogus);
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(0, map.size());
        EXPECT_TRUE(map.empty());
    }
    
    TEST(CacheMap, remove) {
        Socket* s = new Socket(0,0,0);
        Cache* expected = new Cache();
        map.put(s, expected);

        Cache* actual = map.get(s);
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(1, map.size());
        EXPECT_FALSE(map.empty());
        
        map.remove(s);
        expected = NULL;
        actual = map.get(s);
        EXPECT_EQ(expected, actual);
        EXPECT_EQ(0, map.size());
        EXPECT_TRUE(map.empty());
    }


}
