/* 
 * File:   SocketCollectionTest.h
 * Author: rbuck
 *
 * Created on December 21, 2010, 10:40 AM
 */

#ifndef _SOCKETCOLLECTIONTEST_H
#define	_SOCKETCOLLECTIONTEST_H

#include "gtest/gtest.h"
#include "SocketCollection.h"
#include "AddressPort.h"
#include "Utils.h"

#define collection SocketCollection::instance()

using namespace std;

namespace {

    class SocketCollectionTest : public ::testing::Test {
    protected:

        virtual void SetUp() {
            collection.reset();
            PortManagerFactory::instance().create().reset();
            SocketManager::instance().reset();
        }

        virtual void TearDown() {

        }

        virtual void mysleep() {
            usleep(10);
        }
    };

    TEST_F(SocketCollectionTest, DNE) {
        ASSERT_TRUE(NULL == collection.get_by_id(9));
    }

    TEST_F(SocketCollectionTest, SizePushRemove) {
        //printf("Heap size = %d\n", (int) GC_get_heap_size());

        int number = 1000;

        collection.reset();
        ASSERT_EQ(0, collection.size());

        int ids[number];

        for (int i = 0; i < number; ++i) {
            Socket* s = new Socket(i, i, i);
            mysleep();
            ids[i] = s->get_socket_id();
            collection.push(s);
            int size = collection.size();
            ASSERT_EQ(i + 1, size);
        }

        for (int i = 0; i < number; ++i) {
            int id = ids[i];
            Socket* s = collection.get_by_id(id);
            ASSERT_TRUE(s != NULL);
            ASSERT_EQ(id, s->get_socket_id());

            collection.remove(s);
            ASSERT_EQ(number - i - 1, collection.size());
            ASSERT_EQ(NULL, collection.get_by_id(id));
        }

        collection.clear();
        ASSERT_EQ(0, collection.size());

//        printf("Heap size = %d\n", (int) GC_get_heap_size());

    }

    TEST_F(SocketCollectionTest, Integer) {

        int number = 1000;
        Socket * sockets[number];

        for (int i = 0; i < number; i++) {
            sockets[i] = new Socket(i, i, i);
            mysleep();
            collection.push(sockets[i]);
        }

        ASSERT_EQ(number, collection.size());

        for (int i = 0; i < number; i++) {
            Socket* expected = sockets[i];
            Socket* result = collection.get_by_id(expected->get_socket_id());
            ASSERT_EQ(expected->get_socket_id(), result->get_socket_id());
            ASSERT_EQ(expected->get_domain(), result->get_domain());
            ASSERT_EQ(expected->get_type(), result->get_type());
            ASSERT_EQ(expected->get_protocol(), result->get_protocol());
            ASSERT_EQ(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
            ASSERT_EQ(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());


            // try a few unused ones
            int invalidID = SocketManager::instance().get();
            ASSERT_EQ(NULL, collection.get_by_id(invalidID));
            SocketManager::instance().remove(invalidID);
        }

    }

    TEST_F(SocketCollectionTest, LocalOnly) {

        int number = 1000;
        Socket * sockets[number];
        AddressPort * aps[number];

        for (int i = 0; i < number; i++) {
            sockets[i] = new Socket(i, i, i);
            mysleep();
            gcstring address = "192.0.0.";
            int n = i % 255;
            address.append(Utils::itoa(n));
            aps[i] = new AddressPort(address, i);

            //            cause update to happen
            collection.push(sockets[i]);
            sockets[i]->set_local_address_port(aps[i]);
        }

        ASSERT_EQ(number, collection.size());

        for (int i = 0; i < number; i++) {
            Socket* expected = sockets[i];
            AddressPort* temp = sockets[i]->get_local_address_port();
            Socket* result = collection.get_by_local_ap(temp);

            ASSERT_EQ(expected->get_socket_id(), result->get_socket_id());
            ASSERT_EQ(expected->get_domain(), result->get_domain());
            ASSERT_EQ(expected->get_type(), result->get_type());
            ASSERT_EQ(expected->get_protocol(), result->get_protocol());
            ASSERT_EQ(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
            ASSERT_EQ(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

            temp = new AddressPort("192.1.0.1", i);
            ASSERT_EQ(NULL, collection.get_by_local_ap(temp));
        }
    }

    TEST_F(SocketCollectionTest, LocalAndRemote) {

        int number = 1000;
        Socket * sockets[number];
        AddressPort * locals[number];
        AddressPort * remotes[number];

        for (int i = 0; i < number; i++) {
            sockets[i] = new Socket(i, i, i);
            mysleep();
            gcstring local_address = "192.0.0.";
            gcstring remote_address = "192.1.1.";
            int n = i % 255;
            local_address.append(Utils::itoa(n));
            remote_address.append(Utils::itoa(n));
            locals[i] = new AddressPort(local_address, i);
            remotes[i] = new AddressPort(remote_address, i + 5);
            sockets[i]->set_local_address_port(locals[i]);
            sockets[i]->set_remote_address_port(remotes[i]);
            collection.push(sockets[i]);
        }

        ASSERT_EQ(number, collection.size());

        for (int i = 0; i < number; i++) {
            Socket* expected = sockets[i];
            AddressPort* local = sockets[i]->get_local_address_port();
            AddressPort* remote = sockets[i]->get_remote_address_port();

            Socket* result = collection.get_by_local_and_remote_ap(local, remote);

            ASSERT_EQ(expected->get_socket_id(), result->get_socket_id());
            ASSERT_EQ(expected->get_domain(), result->get_domain());
            ASSERT_EQ(expected->get_type(), result->get_type());
            ASSERT_EQ(expected->get_protocol(), result->get_protocol());
            ASSERT_EQ(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
            ASSERT_EQ(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

            remote = new AddressPort("0.0.0.0", i + 6);
            ASSERT_EQ(NULL, collection.get_by_local_and_remote_ap(local, remote));
        }
    }

    TEST_F(SocketCollectionTest, MIX) {

        int number = 1000;
        Socket * sockets[number];
        AddressPort * locals[number];
        AddressPort * remotes[number];

        for (int i = 0; i < number; i++) {
            sockets[i] = new Socket(i, i, i);
            mysleep();
            gcstring local_address = "192.0.0.";
            gcstring remote_address = "192.1.1.";
            int n = i % 255;
            local_address.append(Utils::itoa(n));
            remote_address.append(Utils::itoa(n));
            locals[i] = new AddressPort(local_address, i);
            remotes[i] = new AddressPort(remote_address, i + 5);
            sockets[i]->set_local_address_port(locals[i]);
            sockets[i]->set_remote_address_port(remotes[i]);
            collection.push(sockets[i]);
        }

        ASSERT_EQ(number, collection.size());

        for (int i = 0; i < number; i++) {

            // get by int
            Socket* expected = sockets[i];
            Socket* result = collection.get_by_id(expected->get_socket_id());
            ASSERT_EQ(expected->get_socket_id(), result->get_socket_id());
            ASSERT_EQ(expected->get_domain(), result->get_domain());
            ASSERT_EQ(expected->get_type(), result->get_type());
            ASSERT_EQ(expected->get_protocol(), result->get_protocol());
            ASSERT_EQ(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
            ASSERT_EQ(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

            // try a few unused ones
            u_int16_t unused = SocketManager::instance().get();
            ASSERT_TRUE(NULL == collection.get_by_id(unused));
            SocketManager::instance().remove(unused);

            // get by local && remote
            expected = sockets[i];
            AddressPort* local = sockets[i]->get_local_address_port();
            AddressPort* remote = sockets[i]->get_remote_address_port();

            result = collection.get_by_local_and_remote_ap(local, remote);

            ASSERT_EQ(expected->get_socket_id(), result->get_socket_id());
            ASSERT_EQ(expected->get_domain(), result->get_domain());
            ASSERT_EQ(expected->get_type(), result->get_type());
            ASSERT_EQ(expected->get_protocol(), result->get_protocol());
            ASSERT_EQ(expected->get_local_address_port()->to_s(), result->get_local_address_port()->to_s());
            ASSERT_EQ(expected->get_remote_address_port()->to_s(), result->get_remote_address_port()->to_s());

            remote = new AddressPort("0.0.0.0", i + 6);
            ASSERT_EQ(NULL, collection.get_by_local_and_remote_ap(local, remote));
        }
    }
}

#endif	/* _SOCKETCOLLECTIONTEST_H */

