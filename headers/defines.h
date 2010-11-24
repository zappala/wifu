/* 
 * File:   includes.h
 * Author: rbuck
 *
 * Created on October 22, 2010, 11:05 AM
 */

#ifndef _INCLUDES_H
#define	_INCLUDES_H

#define type_name(X) typeid(X).name()
#define SIG_ENQUEUE_EVENT 50
#define SIG_CANCEL_EVENT 51
#define NANOSECONDS_IN_SECONDS 1000000000
#define PAYLOAD_SIZE 1500

// message defines
#define SOCKET_STRING "socket"
#define FILE_STRING "file"
#define NAME_STRING "name"
#define RETURN_VALUE_STRING "return_value"

// WIFU_PORT is created from the ASCII decimal values of WIFU
// Each number in the port corresponds to one of the letters in 'WIFU'
// by adding up the decimal values of each letter until the value is less than 10
// For example, 'W' is ASCII value 87, 8 + 7 = 15 (which is not less than 10)
// 1 + 5 = 6 (which is less than 10), and thus the first letter in the port number is 6.
#define WIFU_PORT 6174

// We could also make the wifu port the phone digits associated with the letters:
// 9438
typedef const char * event_name;

#endif	/* _INCLUDES_H */

