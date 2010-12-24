/* 
 * File:   AlreadyBoundToAddressPortVisitorTest.h
 * Author: rbuck
 *
 * Created on December 24, 2010, 10:05 AM
 */

#ifndef ALREADYBOUNDTOADDRESSPORTVISITORTEST_H
#define	ALREADYBOUNDTOADDRESSPORTVISITORTEST_H

#include "../headers/visitors/AlreadyBoundToAddressPortVisitor.h"
#include "../headers/Socket.h"
#include "../headers/SocketCollection.h"
#include "../headers/Utils.h"

#include "UnitTest++.h"

#define sc SocketCollection::instance()
#define count 256

using namespace std;

namespace {

    SUITE(AlreadyBoundToAddressPortVisitorTest) {

        TEST(PortCheckUnique) {
            sc.clear();
            Socket * sockets[count];

            string address = "192.168.0.1";

            for (int i = 0; i < count; i++) {
                int port = i;
                AddressPort* local = new AddressPort(address, port);
                sockets[i] = new Socket(i, i, i, local);
                sc.push(sockets[i]);
            }

            for(int port = count; port < count*2; port++) {
                AddressPort to_check(address, port);
                AlreadyBoundToAddressPortVisitor v(&to_check);

                sc.accept(&v);
                CHECK(!v.is_bound());
            }


            for(int i = 0; i < count; i++) {
                delete sockets[i];
            }
        }

        TEST(PortCheckNotUnique) {
            sc.clear();
            Socket * sockets[count];

            string address = "192.168.0.1";

            for (int i = 0; i < count; i++) {
                int port = i;
                AddressPort* local = new AddressPort(address, port);
                sockets[i] = new Socket(i, i, i, local);
                sc.push(sockets[i]);
            }

            for(int port = 0; port < count; port++) {
                AddressPort to_check(address, port);
                AlreadyBoundToAddressPortVisitor v(&to_check);

                sc.accept(&v);
                CHECK(v.is_bound());
            }


            for(int i = 0; i < count; i++) {
                delete sockets[i];
            }
        }

        TEST(AddressCheckUnique) {
            sc.clear();
            Socket * sockets[count];

            int port = 5000;

            for (int i = 0; i < count; i++) {
                string subnet = Utils::itoa(i);
                string address = "192.168.0.";
                address.append(subnet);
                
                AddressPort* local = new AddressPort(address, port);
                sockets[i] = new Socket(i, i, i, local);
                sc.push(sockets[i]);
            }

            for(int i = count; i < count; i++) {
                string address = Utils::itoa(i);
                string rest = ".168.0.1";

                address.append(rest);

                AddressPort to_check(address, port);
                AlreadyBoundToAddressPortVisitor v(&to_check);

                sc.accept(&v);
                CHECK(!v.is_bound());
            }


            for(int i = 0; i < count; i++) {
                delete sockets[i];
            }
        }

        TEST(AddressCheckNotUnique) {
            sc.clear();
            Socket * sockets[count];

            int port = 5000;

            for (int i = 0; i < count; i++) {
                string subnet = Utils::itoa(i);
                string address = "192.168.0.";
                address.append(subnet);

                AddressPort* local = new AddressPort(address, port);
                sockets[i] = new Socket(i, i, i, local);
                sc.push(sockets[i]);
            }

            for(int i = count; i < count; i++) {
                string subnet = Utils::itoa(i);
                string address = "192.168.0.";
                address.append(subnet);

                AddressPort to_check(address, port);
                AlreadyBoundToAddressPortVisitor v(&to_check);

                sc.accept(&v);
                CHECK(v.is_bound());
            }


            for(int i = 0; i < count; i++) {
                delete sockets[i];
            }
        }
    }
}

#endif	/* ALREADYBOUNDTOADDRESSPORTVISITORTEST_H */

