/* 
 * File:   AlreadyBoundToAddressPortVisitorTest.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 10:05 AM
 */

#ifndef ALREADYBOUNDTOADDRESSPORTVISITORTEST_H
#define	ALREADYBOUNDTOADDRESSPORTVISITORTEST_H

#include "visitors/AlreadyBoundToAddressPortVisitor.h"
#include "Socket.h"
#include "SocketCollection.h"
#include "Utils.h"

#include "gtest/gtest.h"

#define sc SocketCollection::instance()
#define count 256

using namespace std;

namespace {

    TEST(AlreadyBoundToAddressPortVisitorTest, PortCheckUnique) {
        sc.clear();
        Socket * sockets[count];

        gcstring address = "192.168.0.1";

        for (int i = 0; i < count; i++) {
            int port = i;
            AddressPort* local = new AddressPort(address, port);
            sockets[i] = new Socket(i, i, i, local);
            sc.push(sockets[i]);
        }

        for (int port = count; port < count * 2; port++) {
            AddressPort to_check(address, port);
            AlreadyBoundToAddressPortVisitor v(&to_check);

            sc.accept(&v);
            ASSERT_TRUE(!v.is_bound());
        }


        for (int i = 0; i < count; i++) {
            delete sockets[i];
        }
    }

    TEST(AlreadyBoundToAddressPortVisitorTest, PortCheckNotUnique) {
        sc.clear();
        Socket * sockets[count];

        gcstring address = "192.168.0.1";

        for (int i = 0; i < count; i++) {
            int port = i;
            AddressPort* local = new AddressPort(address, port);
            sockets[i] = new Socket(i, i, i, local);
            sc.push(sockets[i]);
        }

        for (int port = 0; port < count; port++) {
            AddressPort to_check(address, port);
            AlreadyBoundToAddressPortVisitor v(&to_check);

            sc.accept(&v);
            ASSERT_TRUE(v.is_bound());
        }


        for (int i = 0; i < count; i++) {
            delete sockets[i];
        }
    }

    TEST(AlreadyBoundToAddressPortVisitorTest, AddressCheckUnique) {
        sc.clear();
        Socket * sockets[count];

        int port = 5000;

        for (int i = 0; i < count; i++) {
            gcstring subnet = Utils::itoa(i);
            gcstring address = "192.168.0.";
            address.append(subnet);

            AddressPort* local = new AddressPort(address, port);
            sockets[i] = new Socket(i, i, i, local);
            sc.push(sockets[i]);
        }

        for (int i = count; i < count; i++) {
            gcstring address = Utils::itoa(i);
            gcstring rest = ".168.0.1";

            address.append(rest);

            AddressPort to_check(address, port);
            AlreadyBoundToAddressPortVisitor v(&to_check);

            sc.accept(&v);
            ASSERT_TRUE(!v.is_bound());
        }


        for (int i = 0; i < count; i++) {
            delete sockets[i];
        }
    }

    TEST(AlreadyBoundToAddressPortVisitorTest, AddressCheckNotUnique) {
        sc.clear();
        Socket * sockets[count];

        int port = 5000;

        for (int i = 0; i < count; i++) {
            gcstring subnet = Utils::itoa(i);
            gcstring address = "192.168.0.";
            address.append(subnet);

            AddressPort* local = new AddressPort(address, port);
            sockets[i] = new Socket(i, i, i, local);
            sc.push(sockets[i]);
        }

        for (int i = count; i < count; i++) {
            gcstring subnet = Utils::itoa(i);
            gcstring address = "192.168.0.";
            address.append(subnet);

            AddressPort to_check(address, port);
            AlreadyBoundToAddressPortVisitor v(&to_check);

            sc.accept(&v);
            ASSERT_TRUE(v.is_bound());
        }


        for (int i = 0; i < count; i++) {
            delete sockets[i];
        }
    }
}

#endif	/* ALREADYBOUNDTOADDRESSPORTVISITORTEST_H */

