/* 
 * File:   SocketCollectionTest.h
 * Author: rbuck
 *
 * Created on December 21, 2010, 10:40 AM
 */

#ifndef _SOCKETCOLLECTIONTEST_H
#define	_SOCKETCOLLECTIONTEST_H

#include "UnitTest++.h"

#include "../headers/SocketCollection.h"

#define collection SocketCollection::instance()

using namespace std;

namespace {

	SUITE(SocketCollectionTest) {
            TEST(DNE) {
                collection.clear();
                CHECK(NULL == collection.get_by_id(9));
            }

            TEST(Integer) {
                collection.clear();

                int number = 1000;
                Socket* sockets[number];

                sockets[0] = new Socket(0,0,0);
                delete sockets[0];

                for(int i = 0; i < number; i++) {
                    sockets[i] = new Socket(i, i, i);
                    collection.push(sockets[i]);
                }

                CHECK_EQUAL(number, collection.size());



                


                for(int i = 0; i< number; i++) {
                    delete sockets[i];
                }

            }

            TEST(LocalOnly) {

            }

            TEST(LocalAndRemote) {
                
            }

            TEST(MIX) {
                
            }
        }

}

#endif	/* _SOCKETCOLLECTIONTEST_H */

