/* 
 * File:   HashSetTest.h
 * Author: rbuck
 *
 * Created on December 23, 2010, 11:34 AM
 */

#ifndef HASHSETTEST_H
#define	HASHSETTEST_H

#include "HashSet.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "events/Event.h"
#include "events/TimeoutEvent.h"

using namespace std;

namespace {

    TEST(IntHashSetTest, Empty) {
        HashSet<int> set;
        ASSERT_EQ(0, set.size());
    }

    TEST(IntHashSetTest, Clear) {
    	HashSet<int> set;
    	set.insert(5);
    	set.insert(6);
    	set.insert(8);
    	ASSERT_EQ(set.size(), 3);

    	set.clear();
    	ASSERT_EQ(set.size(), 0);
    }

    TEST(IntHashSetTest, InsertContains) {
        HashSet<int> set;
        int zero = 0;
        set.insert(zero);
        ASSERT_TRUE(set.contains(zero));
    }

    TEST(IntHashSetTest, InsertContainsMany) {
        HashSet<int> set;

        int count = 1000;

        for (int i = 0; i < count; i++) {
            set.insert(i);
        }

        for (int i = 0; i < count; i++) {
            ASSERT_TRUE(set.contains(i));
        }
    }

    TEST(IntHashSetTest, Remove) {
        HashSet<int> set;
        int zero = 0;
        set.insert(zero);
        ASSERT_TRUE(set.contains(zero));

        set.remove(zero);
        ASSERT_TRUE(!set.contains(zero));
        ASSERT_EQ(0, set.size());
    }

    TEST(IntHashSetTest, RemoveMany) {
        HashSet<int> set;

        int count = 1000;

        for (int i = 0; i < count; i++) {
            set.insert(i);
        }

        for (int i = 0; i < count; i++) {
            ASSERT_TRUE(set.contains(i));
        }

        for (int i = 0; i < count; i++) {
            if (i % 2 == 0) {
                set.remove(i);
            }
        }

        ASSERT_EQ(500, set.size());
        for (int i = 0; i < 1000; i++) {
            if (i % 2 == 0) {
                ASSERT_TRUE(!set.contains(i));
            } else {
                ASSERT_TRUE(set.contains(i));
            }
        }
    }

    TEST(EventPointerHashSetTest, Empty) {
        HashSet<Event*> set;
        ASSERT_EQ(0, set.size());
    }

    TEST(EventPointerHashSetTest, InsertContains) {
        HashSet<Event*> set;
        Event* e = new TimeoutEvent(0, 1, 0);

        set.insert(e);
        ASSERT_TRUE(set.contains(e));
    }

    TEST(EventPointerHashSetTest, InsertContainsMany) {
        HashSet<Event*> set;

        int count = 1000;
        Event * events[count];

        for (int i = 0; i < count; i++) {
            events[i] = new TimeoutEvent(0, 0, i);
            set.insert(events[i]);
        }

        for (int i = 0; i < count; i++) {
            ASSERT_TRUE(set.contains(events[i]));
        }
    }

    TEST(EventPointerHashSetTest, Remove) {
        HashSet<Event*> set;
        Event* e = new TimeoutEvent(0, 0, 1);

        set.insert(e);
        ASSERT_TRUE(set.contains(e));

        set.remove(e);
        ASSERT_TRUE(!set.contains(e));
        ASSERT_EQ(0, set.size());
    }

    TEST(EventPointerHashSetTest, RemoveMany) {
        HashSet<Event*> set;

        int count = 1000;
        Event * events[count];

        for (int i = 0; i < count; i++) {
            events[i] = new TimeoutEvent(0, 0, i);
            set.insert(events[i]);
        }

        for (int i = 0; i < count; i++) {
            ASSERT_TRUE(set.contains(events[i]));
        }

        for (int i = 0; i < count; i++) {
            if (i % 2 == 0) {
                set.remove(events[i]);
            }
        }

        ASSERT_EQ(count / 2, set.size());
        for (int i = 0; i < count; i++) {
            if (i % 2 == 0) {
                ASSERT_TRUE(!set.contains(events[i]));
            } else {
                ASSERT_TRUE(set.contains(events[i]));
            }
        }
    }
}

#endif	/* HASHSETTEST_H */

