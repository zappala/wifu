/* 
 * File:   HashSetTest.h
 * Author: rbuck
 *
 * Created on December 23, 2010, 11:34 AM
 */

#ifndef HASHSETTEST_H
#define	HASHSETTEST_H

#include "../headers/HashSet.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

namespace {
	TEST(IntHashSetTest, Empty) {
		HashSet<int> set;
		ASSERT_EQ(0, set.size());
	}

	TEST(IntHashSetTest, InsertContains) {
		HashSet<int> set;
		int zero = 0;
		set.insert(zero);
		ASSERT_TRUE(set.contains(zero));
	}

	TEST(IntHashSetTest, InsertContainsMany) {
		HashSet<int> set;

		for(int i = 0; i < 1000; i++) {
			set.insert(i);
		}

		for(int i = 0; i < 1000; i++) {
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

		for(int i = 0; i < 1000; i++) {
			set.insert(i);
		}

		for(int i = 0; i < 1000; i++) {
			ASSERT_TRUE(set.contains(i));
		}

		for(int i = 0; i < 1000; i++) {
			if(i % 2 == 0) {
				set.remove(i);
			}
		}

		ASSERT_EQ(500, set.size());
		for(int i = 0; i < 1000; i++) {
			if(i % 2 == 0) {
				ASSERT_TRUE(!set.contains(i));
			}
			else {
				ASSERT_TRUE(set.contains(i));
			}
		}
	}

	TEST(Event*HashSetTest, Empty) {
		HashSet<Event*> set;
		ASSERT_EQ(0, set.size());
	}

	TEST(Event*HashSetTest, InsertContains) {
		HashSet<Event*> set;
		Event* e = new TimeoutEvent(0,1,0);

		set.insert(e);
		ASSERT_TRUE(set.contains(e));

		delete e;
	}

	TEST(Event*HashSetTest, InsertContainsMany) {
		HashSet<Event*> set;

		int count = 1000;
		Event* events[count];

		for(int i = 0; i < count; i++) {
			events[i] = new TimeoutEvent(0,0,i);
			set.insert(events[i]);
		}

		for(int i = 0; i < count; i++) {
			ASSERT_TRUE(set.contains(events[i]));
		}

		for(int i = 0; i < count; i++) {
			delete events[i];
		}
	}

	TEST(Event*HashSetTest, Remove) {
		HashSet<Event*> set;
		Event* e = new TimeoutEvent(0,0,1);

		set.insert(e);
		ASSERT_TRUE(set.contains(e));

		set.remove(e);
		ASSERT_TRUE(!set.contains(e));
		ASSERT_EQ(0, set.size());
	}

	TEST(Event*HashSetTest, RemoveMany) {
		HashSet<Event*> set;

		int count = 1000;
		Event* events[count];


		for(int i = 0; i < count; i++) {
			events[i] = new TimeoutEvent(0,0,i);
			set.insert(events[i]);
		}

		for(int i = 0; i < count; i++) {
			ASSERT_TRUE(set.contains(events[i]));
		}

		for(int i = 0; i < count; i++) {
			if(i % 2 == 0) {
				set.remove(events[i]);
			}
		}

		ASSERT_EQ(count / 2, set.size());
		for(int i = 0; i < count; i++) {
			if(i % 2 == 0) {
				ASSERT_TRUE(!set.contains(events[i]));
			}
			else {
				ASSERT_TRUE(set.contains(events[i]));
			}
		}

		for(int i = 0; i < count; i++) {
			delete events[i];
		}
	}
}

#endif	/* HASHSETTEST_H */

