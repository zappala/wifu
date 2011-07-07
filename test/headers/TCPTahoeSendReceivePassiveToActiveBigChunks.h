/* 
 * File:   TCPTahoeSendReceivePassiveToActiveBigChunks.h
 * Author: leer
 *
 * Created on June 30, 2011, 8:13 AM
 */

#ifndef _TCPTAHOESENDRECEIVEPASSIVETOACTIVEBIGCHUNKS_H
#define	_TCPTAHOESENDRECEIVEPASSIVETOACTIVEBIGCHUNKS_H

#include <iostream>
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

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void tcp_tahoe_passive_to_active_big_chunks(int protocol, gcstring message);

void* tcp_tahoe_passive_to_active_big_chunks_thread(void* args);

#endif	/* _TCPTAHOESENDRECEIVEPASSIVETOACTIVEBIGCHUNKS_H */

