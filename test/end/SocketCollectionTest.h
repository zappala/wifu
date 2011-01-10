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
#include "../headers/AddressPort.h"

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
            Socket * sockets[number];

            for (int i = 0; i < number; i++) {
                sockets[i] = new Socket(i, i, i);
                collection.push(sockets[i]);
            }

            CHECK_EQUAL(number, collection.size());

            collection.shuffle();

            for (int i = 0; i < number; i++) {
                Socket* expected = sockets[i];
                Socket* result = collection.get_by_id(expected->get_socket());
                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                // try a few unused ones
                u_int16_t unused = SocketManager::instance().get();
                CHECK(NULL == collection.get_by_id(unused));
                SocketManager::instance().remove(unused);
            }

            for (int i = 0; i < number; i++) {
                delete sockets[i];
            }

        }

        TEST(LocalOnly) {
            collection.clear();

            int number = 1000;
            Socket * sockets[number];
            AddressPort * aps[number];

            for (int i = 0; i < number; i++) {
                sockets[i] = new Socket(i, i, i);
                aps[i] = new AddressPort("192.0.0.1", i);
                sockets[i]->set_local_address_port(aps[i]);
                collection.push(sockets[i]);
            }

            CHECK_EQUAL(number, collection.size());

            collection.shuffle();

            for (int i = 0; i < number; i++) {
                Socket* expected = sockets[i];
                AddressPort* temp = sockets[i]->get_local_address_port();
                Socket* result = collection.get_by_local_ap(temp);

                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                temp = new AddressPort("192.1.0.1", i);
                CHECK(NULL == collection.get_by_local_ap(temp));
                delete temp;
            }

            for (int i = 0; i < number; i++) {
                delete sockets[i];
            }
        }

        TEST(LocalAndRemote) {
            collection.clear();

            int number = 1000;
            Socket * sockets[number];
            AddressPort * locals[number];
            AddressPort * remotes[number];

            for (int i = 0; i < number; i++) {
                sockets[i] = new Socket(i, i, i);
                locals[i] = new AddressPort("192.0.0.1", i);
                remotes[i] = new AddressPort("192.1.1.1", i + 5);
                sockets[i]->set_local_address_port(locals[i]);
                sockets[i]->set_remote_address_port(remotes[i]);
                collection.push(sockets[i]);
            }

            CHECK_EQUAL(number, collection.size());

            collection.shuffle();

            for (int i = 0; i < number; i++) {
                Socket* expected = sockets[i];
                AddressPort* local = sockets[i]->get_local_address_port();
                AddressPort* remote = sockets[i]->get_remote_address_port();

                Socket* result = collection.get_by_local_and_remote_ap(local, remote);

                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                remote = new AddressPort("0.0.0.0", i + 6);
                CHECK(NULL == collection.get_by_local_and_remote_ap(local, remote));
                delete remote;
            }

            for (int i = 0; i < number; i++) {
                delete sockets[i];
            }
        }

        TEST(MIX) {
            collection.clear();

            // TODO: cannot set number to be large as it will cause it to sort many times
            int number = 10;
            Socket * sockets[number];
            AddressPort * locals[number];
            AddressPort * remotes[number];

            for (int i = 0; i < number; i++) {
                sockets[i] = new Socket(i, i, i);
                locals[i] = new AddressPort("192.0.0.1", i);
                remotes[i] = new AddressPort("192.1.1.1", i + 5);
                sockets[i]->set_local_address_port(locals[i]);
                sockets[i]->set_remote_address_port(remotes[i]);
                collection.push(sockets[i]);
            }

            CHECK_EQUAL(number, collection.size());

            collection.shuffle();

            for (int i = 0; i < number; i++) {

                // get by int
                Socket* expected = sockets[i];
                Socket* result = collection.get_by_id(expected->get_socket());
                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                // try a few unused ones
                u_int16_t unused = SocketManager::instance().get();
                CHECK(NULL == collection.get_by_id(unused));
                SocketManager::instance().remove(unused);

                // get by local
                expected = sockets[i];
                AddressPort* temp = sockets[i]->get_local_address_port();
                result = collection.get_by_local_ap(temp);

                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                temp = new AddressPort("192.1.0.1", i);
                CHECK(NULL == collection.get_by_local_ap(temp));
                delete temp;
                
                // get by local & remote
                expected = sockets[i];
                AddressPort* local = sockets[i]->get_local_address_port();
                AddressPort* remote = sockets[i]->get_remote_address_port();

                result = collection.get_by_local_and_remote_ap(local, remote);

                CHECK_EQUAL(expected->get_socket(), result->get_socket());
                CHECK_EQUAL(expected->get_domain(), result->get_domain());
                CHECK_EQUAL(expected->get_type(), result->get_type());
                CHECK_EQUAL(expected->get_protocol(), result->get_protocol());
                CHECK_EQUAL(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
                CHECK_EQUAL(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

                remote = new AddressPort("0.0.0.0", i + 6);
                CHECK(NULL == collection.get_by_local_and_remote_ap(local, remote));
                delete remote;
            }

            for (int i = 0; i < number; i++) {
                delete sockets[i];
            }
        }
    }

}

#endif	/* _SOCKETCOLLECTIONTEST_H */
