/* 
 * File:   SimpleTCPClient.cc
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

    cout << "Simple TCP Client" << endl;

    AddressPort ap("127.0.0.1", 5001);
    SimpleTCP tcp(ap);

    AddressPort server("127.0.0.1", 5000);
    tcp.connect(server);


    return (EXIT_SUCCESS);
}

