/* 
 * File:   TCPTahoeSendReceivePassiveToActive.h
 * Author: leer
 *
 * Created on June 30, 2011, 8:13 AM
 */

#ifndef _TCPTAHOESENDRECEIVEPASSIVETOACTIVE_H
#define	_TCPTAHOESENDRECEIVEPASSIVETOACTIVE_H

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"
#include "../headers/AddressPort.h"
#include "../headers/GarbageCollector.h"
#include "../headers/Semaphore.h"
#include "Timer.h"
#include "../headers/RandomStringGenerator.h"

#include "../headers/WiFuTransportIntegrationTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* tahoe_passive_to_active_thread(void* args);

void tahoe_passive_to_active_send_receive_test(int protocol, gcstring message);

#endif	/* _TCPTAHOESENDRECEIVEPASSIVETOACTIVE_H */

