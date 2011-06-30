/* 
 * File:   TCPTahoeSendReceiveActiveToPassiveBigChunks.h
 * Author: leer
 *
 * Created on June 29, 2011, 5:02 PM
 */

#ifndef _TCPTAHOESENDRECEIVEACTIVETOPASSIVEBIGCHUNKS_H
#define	_TCPTAHOESENDRECEIVEACTIVETOPASSIVEBIGCHUNKS_H

#include <iostream>
#include <string>
#include <vector>
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

void* tahoe_active_to_passive_big_chunks_thread(void* args);
void tahoe_active_to_passive_big_chunks(int protocol, string message);

#endif	/* _TCPTAHOESENDRECEIVEACTIVETOPASSIVEBIGCHUNKS_H */

