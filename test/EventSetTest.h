/*
 * EventSetTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef EVENTSETTEST_H_
#define EVENTSETTEST_H_

#include "UnitTest++.h"
#include "../headers/events/EventSet.h"

using namespace std;

namespace {

    SUITE(EventSet) {
        TEST(CONTAINS) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            CHECK_EQUAL(1, set.size());
            CHECK(set.contains(e));

            delete e;
        }

        TEST(DOESNTCONTAIN) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            CHECK_EQUAL(1, set.size());
            CHECK(set.contains(e));

            Event* e2 = new TimeoutEvent(0,2,0);
            CHECK(!set.contains(e2));

            delete e;
            delete e2;
        }

        TEST(REMOVE) {
            EventSet set;
            Event* e = new TimeoutEvent(0,1,0);
            set.add(e);

            CHECK_EQUAL(1, set.size());
            CHECK(set.contains(e));

            Event* e2 = new TimeoutEvent(0,2,0);
            CHECK(!set.contains(e2));


            set.remove(e);
            CHECK_EQUAL(0, set.size());
            CHECK(!set.contains(e));

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

            CHECK_EQUAL(4, set.size());
            CHECK(set.contains(e));
            CHECK(set.contains(e1));
            CHECK(set.contains(e2));
            CHECK(set.contains(e3));

            set.remove(e2);
            CHECK_EQUAL(3, set.size());
            CHECK(!set.contains(e2));

            CHECK(set.contains(e));
            CHECK(set.contains(e1));
            CHECK(set.contains(e3));

            delete e;
            delete e1;
            delete e2;
            delete e3;

        }
    }

}

#endif /* EVENTSETTEST_H_ */
