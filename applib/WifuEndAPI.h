/* 
 * File:   WifuEndAPI.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 3:39 PM
 */

#ifndef _WIFUENDAPI_H
#define	_WIFUENDAPI_H

#include <sys/types.h>
#include <unistd.h>
#include "WifuEndAPILocalSocket.h"
#include "wifu_socket.h"
#define end_socket WifuEndAPILocalSocket::instance()

using namespace std;



#endif /* _WIFUENDAPI_H */

