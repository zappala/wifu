/* 
 * File:   SocketMapTest.h
 * Author: rbuck
 *
 * Created on November 24, 2010, 3:26 PM
 */

#ifndef SOCKETDATAMAPTEST_H
#define	SOCKETDATAMAPTEST_H

#include <iostream>
#include <string>
#include <vector>

#include "headers/UnitTest++.h"
#include "../applib/SocketDataMap.h"
#include "../applib/SocketData.h"

using namespace std;



namespace {

    SUITE(SocketDataMap) {

        TEST(SocketMapTest) {
            SocketData* data = new SocketData();
            data->set_return_value(0);
            
            SocketDataMap::instance().put(0, data);

            data = new SocketData();
            data->set_return_value(2);
            SocketDataMap::instance().put(2, data);

            SocketData* instance = SocketDataMap::instance().get(2);
            CHECK_EQUAL(2, instance->get_return_value());

            instance = SocketDataMap::instance().get(0);
            CHECK_EQUAL(0, instance->get_return_value());

            SocketDataMap::instance().erase_at(0);
            SocketDataMap::instance().delete_at(2);
            SocketDataMap::instance().put(2, instance);

            instance = SocketDataMap::instance().get(2);
            CHECK_EQUAL(0, instance->get_return_value());

            
        }

        TEST(MapTest) {
            map<int, int*> m;

            int a = 0;
            int b = 1;

            m[0] = &a;
            m[1] = &b;
            m[3] = &b;

            int* ptr = m[1];
            m[3] = ptr;

        }
    }
}

#endif	/* SOCKETDATAMAPTEST_H */

