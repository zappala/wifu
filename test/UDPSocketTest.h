/* 
 * File:   UDPSocketTest.h
 * Author: rbuck
 *
 * Created on November 15, 2010, 1:49 PM
 */

#ifndef _UDPSOCKETTEST_H
#define	_UDPSOCKETTEST_H
#include <iostream>
#include <string>
#include <vector>

#include "UnitTest++.h"
#include "../headers/AddressPort.h"
#include "../headers/Packet.h"
#include "../headers/Queue.h"
#include "../headers/UDPSocket.h"


namespace tester {

    TEST(UDPSocketConstructor) {
        string a, b;
        AddressPort addressPort(a, b);
        Packet p((unsigned char*)a.c_str(), 1);
        UDPSocket s;
    }

    TEST(UDPSocketSend) {
    }

}


#endif	/* _UDPSOCKETTEST_H */


