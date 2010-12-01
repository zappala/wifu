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
#define BUFFER_SIZE PAYLOAD_SIZE

// message defines
// method names
#define WIFU_SOCKET_NAME "wifu_socket"
#define WIFU_BIND_NAME "wifu_bind"
#define WIFU_LISTEN_NAME "wifu_listen"
#define WIFU_ACCEPT_NAME "wifu_accept"
#define WIFU_SEND_NAME "wifu_send"
#define WIFU_SENDTO_NAME "wifu_sendto"
#define WIFU_RECV_NAME "wifu_recv"
#define WIFU_RECVFROM_NAME "wifu_recvfrom"
#define WIFU_CONNECT_NAME "wifu_connect"
#define WIFU_GETSOCKOPT_NAME "wifu_getsockopt"
#define WIFU_SETSOCKOPT_NAME "wifu_setsockopt"

// variable names
#define SOCKET_STRING "socket"
#define FILE_STRING "file"
#define NAME_STRING "name"
#define RETURN_VALUE_STRING "return_value"
#define DOMAIN_STRING "domain"
#define TYPE_STRING "type"
#define PROTOCOL_STRING "protocol"
#define ADDRESS_STRING "address"
#define PORT_STRING "port"
#define LENGTH_STRING "length"
#define N_STRING "n"
#define BUFFER_STRING "buffer"
#define FLAGS_STRING "flags"
#define OPTION_NAME_STRING "option_name"
#define OPTION_VALUE_STRING "option_value"
#define LEVEL_STRING "level"



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

