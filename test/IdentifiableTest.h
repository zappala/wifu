/* 
 * File:   IdentifiableTest.h
 * Author: rbuck
 *
 * Created on November 17, 2010, 3:59 PM
 */

#ifndef _IDENTIFIABLETEST_H
#define	_IDENTIFIABLETEST_H

#include "../headers/Identifiable.h"

#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"

using namespace std;

SUITE(Identifiable) {

    TEST(IdentifiableTest) {
        UNITTEST_TIME_CONSTRAINT(1);
        
        Identifiable id0;
        Identifiable id1;

        CHECK(id0.get_id() != id1.get_id());
    }
}

#endif	/* _IDENTIFIABLETEST_H */

