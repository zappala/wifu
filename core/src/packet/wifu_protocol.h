#ifndef _wifu_protocol_h
#define	_wifu_protocol_h

#define WIFU_PROTOCOL 253
#define WIFU_VERSION 1
#define WIFU_LENGTH 4

struct wifuhdr {
    u_int8_t version;
    u_int8_t type;
    u_int8_t protocol;
    u_int8_t length;
};

#endif	// _wifu_protocol_h
