/* 
 * File:   SocketMapTest.h
 * Author: rbuck
 *
 * Created on November 24, 2010, 3:26 PM
 */

#ifndef SOCKETMAPTEST_H
#define	SOCKETMAPTEST_H

#include <iostream>
#include <string>
#include <vector>

#include "headers/UnitTest++.h"
#include "../applib/SocketMap.h"
#include "../applib/SocketData.h"

using namespace std;



namespace {

    SUITE(SocketMap) {

        TEST(SocketMapTest) {
            SocketData* data = new SocketData();
            data->set_return_value(0);
            
            SocketMap::instance().put(0, data);

            data = new SocketData();
            data->set_return_value(2);
            SocketMap::instance().put(2, data);

            SocketData* instance = SocketMap::instance().get(2);
            CHECK_EQUAL(2, instance->get_return_value());

            instance = SocketMap::instance().get(0);
            CHECK_EQUAL(0, instance->get_return_value());

            SocketMap::instance().erase_at(0);
            SocketMap::instance().delete_at(2);
            SocketMap::instance().put(2, instance);

            instance = SocketMap::instance().get(2);
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

#endif	/* SOCKETMAPTEST_H */

