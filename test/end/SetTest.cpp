/* 
 * File:   SetTest.h
 * Author: rbuck
 *
 * Created on December 23, 2010, 11:34 AM
 */

#ifndef SETTEST_H
#define	SETTEST_H

#include "Set.h"

#include <iostream>

#include "gtest/gtest.h"
#include "events/Event.h"
#include "events/framework_events/TimeoutEvent.h"

using namespace std;

namespace {

    TEST(IntSetTest, Empty) {
        Set<int> set;
        ASSERT_EQ(0, set.size());
    }

    TEST(IntSetTest, Clear) {
    	Set<int> set;
    	set.insert(5);

    	set.insert(6);
    	set.insert(8);
    	ASSERT_EQ(set.size(), 3);

    	set.clear();
    	ASSERT_EQ(set.size(), 0);
    }

    TEST(IntSetTest, InsertContains) {
        Set<int> set;
        int zero = 0;
        set.insert(zero);
        ASSERT_TRUE(set.contains(zero));
    }

    TEST(IntSetTest, InsertContainsMany) {
        Set<int> set;

        int count = 1000;

        for (int i = 0; i < count; i++) {
            set.insert(i);
        }

        for (int i = 0; i < count; i++) {
            ASSERT_TRUE(set.contains(i));
        }
    }

    TEST(IntSetTest, Remove) {
        Set<int> set;
        int zero = 0;
        set.insert(zero);
        ASSERT_TRUE(set.contains(zero));

        set.remove(zero);
        ASSERT_TRUE(!set.contains(zero));
        ASSERT_EQ(0, set.size());
    }

    TEST(IntSetTest, RemoveMany) {
        Set<int> set;

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

    TEST(EventPointerSetTest, Empty) {
        Set<Event*> set;
        ASSERT_EQ(0, set.size());
    }

    TEST(EventPointerSetTest, InsertContains) {
        Set<Event*> set;
        Event* e = new TimeoutEvent(0, 1, 0);

        set.insert(e);
        ASSERT_TRUE(set.contains(e));
    }

    TEST(EventPointerSetTest, InsertContainsMany) {
        Set<Event*> set;

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

    TEST(EventPointerSetTest, Remove) {
        Set<Event*> set;
        Event* e = new TimeoutEvent(0, 0, 1);

        set.insert(e);
        ASSERT_TRUE(set.contains(e));

        set.remove(e);
        ASSERT_TRUE(!set.contains(e));
        ASSERT_EQ(0, set.size());
    }

    TEST(EventPointerSetTest, RemoveMany) {
        Set<Event*> set;

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

#endif	/* SETTEST_H */

