#ifndef MULTIPLE_PROTOCOLS
#define	MULTIPLE_PROTOCOLS

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

void mutliple_protocols(gcstring message);

void* tcp_tahoe_thread(void* args);

#endif	/* MULTIPLE_PROTOCOLS */

