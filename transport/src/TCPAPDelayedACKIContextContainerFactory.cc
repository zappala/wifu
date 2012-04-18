/* 
 * File:   TCPAPDelayedACKIContextContainerFactory.cc
 * Author: leer
 * 
 * Created on July 6, 2011, 2:03 PM
 */

#include "TCPAPDelayedACKIContextContainerFactory.h"

BasicIContextContainer* TCPAPDelayedACKIContextContainerFactory::get_icontext_container() {
    return new TCPAPDelayedACKIContextContainer();
}