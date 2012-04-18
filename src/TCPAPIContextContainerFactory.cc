/* 
 * File:   TCPAPIContextContainerFactory.cc
 * Author: leer
 * 
 * Created on July 5, 2011, 2:54 PM
 */

#include "TCPAPIContextContainerFactory.h"
#include "contexts/TCPAPIContextContainer.h"

BasicIContextContainer* TCPAPIContextContainerFactory::get_icontext_container() {
    return new TCPAPIContextContainer();
}
