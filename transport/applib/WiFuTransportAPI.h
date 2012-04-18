/* 
 * File:   WiFuTransportAPI.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 3:39 PM
 */

#ifndef _WIFUTRANSPORTAPI_H
#define	_WIFUTRANSPORTAPI_H

#include <sys/types.h>
#include <unistd.h>

#include "WifuTransportFrontEndTranslator.h"
#include "wifu_socket.h"
#define wifu_transport_local_socket WifuTransportFrontEndTranslator::instance()

using namespace std;



#endif /* _WIFUTRANSPORTAPI_H */

