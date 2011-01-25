/*
 * EventSetTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef EVENTSETTEST_H_
#define EVENTSETTEST_H_

#include "gtest/gtest.h"
#include "../headers/events/EventSet.h"

using namespace std;

namespace {

    SUITE(EventSet) {
        TEST(CONTAINS) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            ASSERT_EQ(1, set.size());
            ASSERT_TRUE(set.contains(e));

            delete e;
        }

        TEST(DOESNTCONTAIN) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            ASSERT_EQ(1, set.size());
            ASSERT_TRUE(set.contains(e));

            Event* e2 = new TimeoutEvent(0,2,0);
            ASSERT_TRUE(!set.contains(e2));

            delete e;
            delete e2;
        }

        TEST(REMOVE) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            ASSERT_EQ(1, set.size());
            ASSERT_TRUE(set.contains(e));

            Event* e2 = new TimeoutEvent(0,2,0);
            ASSERT_TRUE(!set.contains(e2));


            set.remove(e);
            ASSERT_EQ(0, set.size());
            ASSERT_TRUE(!set.contains(e));

            delete e;
            delete e2;
        }

        TEST(MULTIPLE) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            Event* e1 = new TimeoutEvent(0,2,0);
            Event* e2 = new TimeoutEvent(0,3,0);
            Event* e3 = new TimeoutEvent(0,4,0);

            set.add(e);
            set.add(e1);
            set.add(e2);
            set.add(e3);

            ASSERT_EQ(4, set.size());
            ASSERT_TRUE(set.contains(e));
            ASSERT_TRUE(set.contains(e1));
            ASSERT_TRUE(set.contains(e2));
            ASSERT_TRUE(set.contains(e3));

            set.remove(e2);
            ASSERT_EQ(3, set.size());
            ASSERT_TRUE(!set.contains(e2));

            ASSERT_TRUE(set.contains(e));
            ASSERT_TRUE(set.contains(e1));
            ASSERT_TRUE(set.contains(e3));

            delete e;
            delete e1;
            delete e2;
            delete e3;

        }
    }

}

#endif /* EVENTSETTEST_H_ */
