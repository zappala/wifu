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

    SUITE(HashSetInteger) {

        TEST(Empty) {
            HashSet<int> set;
            ASSERT_EQ(0, set.size());
        }

        TEST(InsertContains) {
            HashSet<int> set;
            int zero = 0;
            set.insert(zero);
            CHECK(set.contains(zero));
        }

        TEST(InsertContainsMany) {
            HashSet<int> set;
            
            for(int i = 0; i < 1000; i++) {
                set.insert(i);
            }
            
            for(int i = 0; i < 1000; i++) {
                CHECK(set.contains(i));
            }
        }

        TEST(Remove) {
            HashSet<int> set;
            int zero = 0;
            set.insert(zero);
            CHECK(set.contains(zero));

            set.remove(zero);
            CHECK(!set.contains(zero));
            ASSERT_EQ(0, set.size());
        }

        TEST(RemoveMany) {
            HashSet<int> set;

            for(int i = 0; i < 1000; i++) {
                set.insert(i);
            }

            for(int i = 0; i < 1000; i++) {
                CHECK(set.contains(i));
            }

            for(int i = 0; i < 1000; i++) {
                if(i % 2 == 0) {
                    set.remove(i);
                }
            }

            ASSERT_EQ(500, set.size());
            for(int i = 0; i < 1000; i++) {
                if(i % 2 == 0) {
                    CHECK(!set.contains(i));
                }
                else {
                    CHECK(set.contains(i));
                }
            }
        }
    }

    SUITE(HashSetPointer) {
        TEST(Empty) {
            HashSet<Event*> set;
            ASSERT_EQ(0, set.size());
        }

        TEST(InsertContains) {
            HashSet<Event*> set;
            Event* e = new TimeoutEvent(0,1,0);

            set.insert(e);
            CHECK(set.contains(e));

            delete e;
        }

        TEST(InsertContainsMany) {
            HashSet<Event*> set;

            int count = 1000;
            Event* events[count];

            for(int i = 0; i < count; i++) {
                events[i] = new TimeoutEvent(0,0,i);
                set.insert(events[i]);
            }

            for(int i = 0; i < count; i++) {
                CHECK(set.contains(events[i]));
            }

            for(int i = 0; i < count; i++) {
                delete events[i];
            }
        }

        TEST(Remove) {
            HashSet<Event*> set;
            Event* e = new TimeoutEvent(0,0,1);
            
            set.insert(e);
            CHECK(set.contains(e));

            set.remove(e);
            CHECK(!set.contains(e));
            ASSERT_EQ(0, set.size());
        }

        TEST(RemoveMany) {
            HashSet<Event*> set;

            int count = 1000;
            Event* events[count];


            for(int i = 0; i < count; i++) {
                events[i] = new TimeoutEvent(0,0,i);
                set.insert(events[i]);
            }

            for(int i = 0; i < count; i++) {
                CHECK(set.contains(events[i]));
            }

            for(int i = 0; i < count; i++) {
                if(i % 2 == 0) {
                    set.remove(events[i]);
                }
            }

            ASSERT_EQ(count / 2, set.size());
            for(int i = 0; i < count; i++) {
                if(i % 2 == 0) {
                    CHECK(!set.contains(events[i]));
                }
                else {
                    CHECK(set.contains(events[i]));
                }
            }

            for(int i = 0; i < count; i++) {
                delete events[i];
            }
        }
    }
}

#endif	/* HASHSETTEST_H */

