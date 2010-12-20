/* 
 * File:   SimpleTCPServer.cc
 * Author: rbuck
 *
 * Created on December 17, 2010, 11:30 AM
 */

#include <stdlib.h>
#include <iostream>


#include "SimpleTCP.h"
#include "../headers/AddressPort.h"
#include "../headers/Semaphore.h"

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Simple TCP Server" << endl;

    AddressPort ap("127.0.0.1", 5000);
    SimpleTCP tcp(ap);

    Semaphore sem;
    sem.init(0);
    sem.wait();

    return (EXIT_SUCCESS);
}

