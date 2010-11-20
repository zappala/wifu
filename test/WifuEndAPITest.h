/* 
 * File:   WifuEndAPITest.h
 * Author: rbuck
 *
 * Created on November 20, 2010, 1:29 PM
 */

#ifndef _WIFUENDAPITEST_H
#define	_WIFUENDAPITEST_H

#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"
#include "../applib/WifuEndAPI.h"

using namespace std;

namespace {

    SUITE(WifuEndTest) {

        TEST(WifuEndTest) {
            int result = wifu_sendto(0, 0, 0, 0, 0, 0);
            cout << result << endl;
            CHECK(result == 0);
        }

    }
}


#endif	/* _WIFUENDAPITEST_H */

