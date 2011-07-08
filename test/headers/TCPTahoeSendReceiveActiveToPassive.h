/* 
 * File:   TCPTahoeSendReceiveActiveToPassive.h
 * Author: leer
 *
 * Created on June 29, 2011, 4:32 PM
 */

#ifndef _TCPTAHOESENDRECEIVEACTIVETOPASSIVE_H
#define	_TCPTAHOESENDRECEIVEACTIVETOPASSIVE_H

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
#include "../headers/packet/TCPPacket.h"

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* tahoe_active_to_passive_thread_with_close(void* args);

void tahoe_active_to_passive_test_with_close(int protocol, gcstring message);

#endif	/* _TCPTAHOESENDRECEIVEACTIVETOPASSIVE_H */

